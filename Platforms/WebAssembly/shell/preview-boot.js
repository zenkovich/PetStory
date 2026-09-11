// Boot for the game preview client: the same session working copy as the editor
// next door, streamed into MEMFS, and the emscripten Module the parent page
// drives. Everything the editor shell does around the engine (chat, browser,
// bottom bar) belongs to the parent — this page is the game and nothing else.

console.log('[preview] game preview shell');

// The session is the tab's, not the frame's: sessionStorage is shared with the
// parent page, so the client opens the very working copy the editor is editing.
var sid = sessionStorage.getItem('o2sid');
if (!sid) {
    sid = Math.random().toString(36).slice(2) + Math.random().toString(36).slice(2);
    sessionStorage.setItem('o2sid', sid);
}
window.o2Base = location.pathname.replace(/\/[^\/]*$/, '');
document.cookie = 'o2sid=' + sid + '; Path=' + (window.o2Base || '/') + '; SameSite=Strict';
// Defined ⇒ the engine's WebFS mirrors every MEMFS mutation back to the server,
// so an asset rebuilt here is on disk for the editor and the agent as well
window.o2fsEndpoint = window.o2Base + '/api';

var statusEl = document.getElementById('status');
var statusText = document.getElementById('status-text');
var progressBar = document.querySelector('#progress > span');
function setStatus(text, frac) {
    // the parent page shows the same thing over the pane, so a switch to the
    // game reads as loading rather than as an empty rectangle
    try {
        parent.postMessage({ o2preview: 'progress', text: text || '', frac: typeof frac === 'number' ? frac : null },
                           location.origin);
    } catch (e) {}
    if (!text) { statusEl.classList.add('hidden'); return; }
    statusEl.classList.remove('hidden');
    statusText.textContent = text;
    if (typeof frac === 'number')
        progressBar.style.width = (frac * 100).toFixed(1) + '%';
}

function loadSnapshot(done, fail) {
    fetch(o2Base + '/api/fs/snapshot').then(function (resp) {
        if (!resp.ok) throw new Error('snapshot HTTP ' + resp.status);
        var total = +resp.headers.get('X-Uncompressed-Length') || 0;
        var reader = resp.body.getReader();
        var chunks = [], loaded = 0;
        function pump() {
            return reader.read().then(function (r) {
                if (r.done) return;
                chunks.push(r.value);
                loaded += r.value.length;
                setStatus('Loading project… ' + (loaded / 1048576).toFixed(1) + ' MB', total ? loaded / total : 0);
                return pump();
            });
        }
        return pump().then(function () {
            var buf = new Uint8Array(loaded), off = 0;
            for (var i = 0; i < chunks.length; i++) { buf.set(chunks[i], off); off += chunks[i].length; }
            return buf;
        });
    }).then(function (buf) {
        var magic = String.fromCharCode.apply(null, buf.subarray(0, 8));
        if (magic !== 'O2SNAP01') throw new Error('bad snapshot magic');
        var indexLen = new DataView(buf.buffer, buf.byteOffset + 8, 4).getUint32(0, true);
        var index = JSON.parse(new TextDecoder().decode(buf.subarray(12, 12 + indexLen)));
        var off = 12 + indexLen;

        setStatus('Unpacking project…', 1);
        var FS = Module.FS;
        var madeDirs = {};
        function mkdirs(dir) {
            if (madeDirs[dir]) return;
            FS.mkdirTree(dir);
            madeDirs[dir] = true;
        }
        for (var i = 0; i < index.files.length; i++) {
            var f = index.files[i];
            var full = '/project/' + f.p;
            mkdirs(full.substring(0, full.lastIndexOf('/')));
            FS.writeFile(full, buf.subarray(off, off + f.s));
            if (f.m) try { FS.utime(full, f.m, f.m); } catch (e) {}
            off += f.s;
        }
        mkdirs('/project/Bin/WebAssembly');
        console.log('[preview] snapshot unpacked:', index.files.length, 'files');
        done();
    }).catch(function (e) {
        // A dropped connection on the way in is not worth losing the client
        // over: the stream is idempotent, so ask again before giving up.
        console.error('[preview] snapshot failed', e);
        if ((loadSnapshot.tries = (loadSnapshot.tries || 0) + 1) <= 2) {
            setStatus('Reconnecting… (' + loadSnapshot.tries + ')');
            setTimeout(function () { loadSnapshot(done, fail); }, 800 * loadSnapshot.tries);
            return;
        }
        setStatus('Failed to load the project: ' + e.message);
        fail(e);
    });
}

// The agent screenshots this canvas from the parent page, and drives the game
// with synthetic events: both need the same patches the editor page applies.
(function () {
    var orig = HTMLCanvasElement.prototype.getContext;
    HTMLCanvasElement.prototype.getContext = function (type, attrs) {
        if (this.id === 'canvas' && (type === 'webgl2' || type === 'webgl'))
            attrs = Object.assign({}, attrs, { preserveDrawingBuffer: true });
        return orig.call(this, type, attrs);
    };
    var sc = Element.prototype.setPointerCapture;
    Element.prototype.setPointerCapture = function (id) { try { return sc.call(this, id); } catch (e) {} };
    var rc = Element.prototype.releasePointerCapture;
    Element.prototype.releasePointerCapture = function (id) { try { return rc.call(this, id); } catch (e) {} };
})();

// What the engine printed, for the agent's read_log — the parent reads this
// array out of the frame the same way it reads its own
var engineLogLines = [];
function engineLog(kind, text) {
    engineLogLines.push((kind === 'err' ? 'ERR ' : '') + text);
    if (engineLogLines.length > 500) engineLogLines.splice(0, engineLogLines.length - 500);
}

function isWasmCrash(err, message) {
    if (typeof WebAssembly !== 'undefined' && err instanceof WebAssembly.RuntimeError) return true;
    var t = String(message || (err && err.message) || '');
    return /RuntimeError|memory access out of bounds|null function or function signature|unreachable|table index is out of bounds|Aborted\(/.test(t);
}

// A crash here is the game's, not the editor's: tell the parent, which shows it
// and decides whether to reload the client
var crashHandled = false;
function handleCrash(message, stack) {
    if (crashHandled) return;
    crashHandled = true;
    console.error('[preview.crash]', message, stack);
    setStatus('The game crashed');
    try {
        parent.postMessage({ o2preview: 'crash', message: String(message || 'wasm crash'),
                             stack: String(stack || '').slice(0, 8000) }, location.origin);
    } catch (e) {}
}
window.addEventListener('error', function (e) {
    if (isWasmCrash(e.error, e.message)) {
        e.preventDefault();
        handleCrash(e.message || String(e.error), e.error && e.error.stack);
    }
});
window.addEventListener('unhandledrejection', function (e) {
    if (isWasmCrash(e.reason)) {
        e.preventDefault();
        handleCrash(String((e.reason && e.reason.message) || e.reason), e.reason && e.reason.stack);
    }
});

// Heartbeat the parent can read: it tells whether the client is still ticking
// while the editor is in front (it should be — it is a running game, not a tab
// that was closed).
window.__o2Ticks = 0;
(function tick() { window.__o2Ticks++; requestAnimationFrame(tick); })();

var Module = {
    canvas: (function () {
        var c = document.getElementById('canvas');
        c.addEventListener('webglcontextlost', function (e) {
            console.error('[preview] webglcontextlost');
            setStatus('WebGL context lost — restart the client');
            e.preventDefault();
        }, false);
        return c;
    })(),
    print: function (text) { console.log('[game.out]', text); engineLog('out', text); },
    printErr: function (text) { console.error('[game.err]', text); engineLog('err', text); },
    setStatus: function (text) {
        if (text) console.log('[preview.setStatus]', text);
        var m = text && text.match(/([^\(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
        if (m) setStatus('Downloading the client… ' + m[1].trim().replace(/^Downloading data\.\.\.$/, ''),
                         parseFloat(m[2]) / parseFloat(m[4]));
        else if (text) setStatus(text);
        else setStatus(null);
    },
    preRun: [function () {
        Module.addRunDependency('project-snapshot');
        loadSnapshot(
            function () { Module.removeRunDependency('project-snapshot'); },
            function () { /* startup halts with the error shown */ });
    }],
    onRuntimeInitialized: function () {
        console.log('[preview.onRuntimeInitialized]');
        setStatus(null);
        try { parent.postMessage({ o2preview: 'ready' }, location.origin); } catch (e) {}
    },
    onAbort: function (reason) { handleCrash('abort: ' + reason, new Error().stack); },
    onExit: function (code) { console.log('[preview.onExit]', code); }
};
setStatus('Loading the game client…');
