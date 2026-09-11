// The preview pane: the game client of this session, running in its own frame.
//
// The page has two faces over one working copy — the editor with the agent, and
// the game with the agent — and the bar switches between them mid-session. Both
// keep running: the client is loaded once and goes on playing behind the editor,
// so switching back is instant and the game keeps its state. It picks up new
// assets the way it always does — rebuild_assets, then restart.
//
// Everything the agent does in preview mode goes through the same tools as in the
// editor; ai.js just aims them at this frame's Module and canvas (see o2Preview).

var o2Preview = (function () {
    var root = document.getElementById('preview');
    var stage = document.getElementById('preview-stage');
    var screen = document.getElementById('preview-screen');
    var hint = document.getElementById('preview-hint');
    var sizeLabel = document.getElementById('pv-size');
    var restartBtn = document.getElementById('pv-restart');

    var frame = null;          // the client; it outlives a switch back to the editor
    var crashed = false;       // a dead client is remounted on the next switch
    var mode = 'editor';
    var ready = false;
    var listeners = [];

    // Portrait sizes; landscape swaps them. "Fit" follows the pane, which is what
    // you want while working, the fixed ones are for checking a real device — and
    // each is drawn in its own body, so the shape you are testing is on screen.
    var DEVICES = [
        { id: 'fit', label: 'Fit to pane', kind: 'fit' },
        { id: '720p', label: 'Desktop 1280×720', w: 1280, h: 720, kind: 'desktop' },
        { id: '1080p', label: 'Desktop 1920×1080', w: 1920, h: 1080, kind: 'desktop' },
        { id: 'iphone-se', label: 'iPhone SE — 375×667', w: 375, h: 667, kind: 'phone' },
        { id: 'iphone-15', label: 'iPhone 15 — 393×852', w: 393, h: 852, kind: 'phone', notch: true },
        { id: 'pixel-8', label: 'Pixel 8 — 412×915', w: 412, h: 915, kind: 'phone', punch: true },
        { id: 'ipad', label: 'iPad — 820×1180', w: 820, h: 1180, kind: 'tablet' },
    ];

    // Silhouettes for the picker: the menu shows what each preset is, not just
    // its numbers
    var SHAPES = {
        fit: '<svg viewBox="0 0 24 24"><rect x="2.5" y="4.5" width="19" height="15" rx="2"/><path d="M7 9h4M7 12h7"/></svg>',
        desktop: '<svg viewBox="0 0 24 24"><rect x="2.5" y="4.5" width="19" height="12" rx="1.5"/><path d="M9 19.5h6M12 16.5v3"/></svg>',
        phone: '<svg viewBox="0 0 24 24"><rect x="7.5" y="2.5" width="9" height="19" rx="2"/><path d="M10.6 4.6h2.8"/></svg>',
        tablet: '<svg viewBox="0 0 24 24"><rect x="5.5" y="2.5" width="13" height="19" rx="2"/><circle cx="12" cy="19" r=".9"/></svg>',
    };
    var device = DEVICES[0];
    var orientation = 'landscape';  // meaningful for the fixed sizes only

    function emit(kind) { listeners.forEach(function (fn) { try { fn(kind); } catch (e) {} }); }

    // The screen gets a body around it (built here so a UI change needs no wasm
    // relink), and the hint gets a progress bar for the client's own loading.
    var device_el = document.createElement('div');
    device_el.id = 'preview-device';
    screen.parentNode.insertBefore(device_el, screen);
    device_el.appendChild(screen);
    var bezel = document.createElement('div');
    bezel.className = 'pv-bezel';
    bezel.innerHTML = '<span class="pv-notch"></span><span class="pv-speaker"></span><span class="pv-home"></span>';
    device_el.appendChild(bezel);

    var progressWrap = document.createElement('div');
    progressWrap.className = 'pv-progress';
    progressWrap.innerHTML = '<span></span>';
    var progressBar = progressWrap.firstChild;
    hint.parentNode.insertBefore(progressWrap, hint.nextSibling);

    function setHint(text, frac) {
        if (!text) {
            hint.style.display = 'none';
            progressWrap.style.display = 'none';
            return;
        }
        hint.style.display = '';
        hint.textContent = text;
        progressWrap.style.display = typeof frac === 'number' ? '' : 'none';
        if (typeof frac === 'number')
            progressBar.style.width = (Math.max(0, Math.min(1, frac)) * 100).toFixed(1) + '%';
    }

    // ---- geometry ----------------------------------------------------
    // The screen keeps its device size in CSS pixels (so the agent's canvas
    // coordinates are the device's) and is scaled down only to fit the pane.
    function layout() {
        var pane = stage.getBoundingClientRect();
        var pad = 24;
        var availW = Math.max(120, pane.width - pad), availH = Math.max(120, pane.height - pad);
        var w, h, scale = 1;
        if (device.id === 'fit') {
            w = Math.round(availW); h = Math.round(availH);
        } else {
            var short = Math.min(device.w, device.h), long = Math.max(device.w, device.h);
            w = orientation === 'landscape' ? long : short;
            h = orientation === 'landscape' ? short : long;
            scale = Math.min(1, availW / w, availH / h);
        }
        screen.style.width = w + 'px';
        screen.style.height = h + 'px';
        device_el.className = 'kind-' + device.kind +
            (device.notch ? ' has-notch' : '') + (device.punch ? ' has-punch' : '') +
            ' ' + (orientation === 'landscape' ? 'is-landscape' : 'is-portrait');
        device_el.style.transform = scale < 1 ? 'scale(' + scale + ')' : '';
        sizeLabel.textContent = w + '×' + h + (scale < 1 ? '  ·  ' + Math.round(scale * 100) + '%' : '');
    }

    window.addEventListener('resize', function () { if (mode === 'preview') layout(); });
    // the pane also changes when the agent panel is resized or folded away
    if (window.ResizeObserver)
        new ResizeObserver(function () { if (mode === 'preview') layout(); }).observe(stage);

    // ---- the client --------------------------------------------------
    function mount() {
        if (frame) return;
        ready = false;
        crashed = false;
        setHint('Loading the game client…', 0);
        frame = document.createElement('iframe');
        frame.id = 'preview-frame';
        frame.setAttribute('title', 'Game preview');
        frame.src = 'GamePreview.html';
        screen.appendChild(frame);
    }

    function unmount() {
        if (!frame) return;
        frame.remove();
        frame = null;
        ready = false;
        emit('unmounted');
    }

    window.addEventListener('message', function (e) {
        if (e.origin !== location.origin || !e.data || !e.data.o2preview) return;
        if (e.data.o2preview === 'ready') {
            ready = true;
            setHint(null);
            emit('ready');
        } else if (e.data.o2preview === 'progress') {
            if (!ready) setHint(e.data.text || 'Loading the game client…',
                                typeof e.data.frac === 'number' ? e.data.frac : undefined);
        } else if (e.data.o2preview === 'crash') {
            ready = false;
            crashed = true;
            setHint('The game crashed: ' + e.data.message);
            emit('crash');
        }
    });

    // Restart with what is on disk now: the client rebuilds nothing by itself, so
    // whatever was already built (by the agent's rebuild_assets, or by the editor
    // before the switch) is what the game starts on.
    function restart() {
        if (!frame) { mount(); return Promise.resolve({ reloaded: true }); }
        var win = frame.contentWindow;
        if (ready && win && win.Module && typeof win.Module._o2_web_restart === 'function') {
            win.Module._o2_web_restart();
            return Promise.resolve({ restarted: true });
        }
        // not up yet (or the client crashed): a fresh frame is the restart
        unmount();
        mount();
        return Promise.resolve({ reloaded: true });
    }

    restartBtn.onclick = function () { restart(); };

    // ---- controls ----------------------------------------------------
    function buildControls() {
        var deviceSlot = document.getElementById('pv-device-slot');
        var dd = document.createElement('span');
        dd.className = 'pv-dd';
        var btn = document.createElement('button');
        btn.innerHTML = '<svg class="icon" viewBox="0 0 16 16"><use href="#i-device"/></svg>' +
                        '<span class="pv-dd-label"></span>' +
                        '<svg class="icon chev" viewBox="0 0 16 16"><use href="#i-chev"/></svg>';
        var menu = document.createElement('div');
        menu.className = 'pv-menu';
        DEVICES.forEach(function (d) {
            var item = document.createElement('button');
            item.innerHTML = '<span class="pv-shape">' + (SHAPES[d.kind] || '') + '</span>' +
                             '<span>' + d.label + '</span>';
            item.onclick = function () {
                device = d;
                menu.classList.remove('open');
                btn.querySelector('.pv-dd-label').textContent = d.id === 'fit' ? 'Fit' : d.label.split(' — ')[0];
                layout();
            };
            menu.appendChild(item);
        });
        btn.onclick = function (e) { e.stopPropagation(); menu.classList.toggle('open'); };
        document.addEventListener('click', function () { menu.classList.remove('open'); });
        dd.appendChild(btn);
        dd.appendChild(menu);
        deviceSlot.appendChild(dd);
        btn.querySelector('.pv-dd-label').textContent = 'Fit';

        var orientSlot = document.getElementById('pv-orient');
        var seg = document.createElement('span');
        seg.className = 'pv-seg';
        [['portrait', 'Portrait'], ['landscape', 'Landscape']].forEach(function (o) {
            var b = document.createElement('button');
            b.textContent = o[1];
            b.dataset.orient = o[0];
            b.onclick = function () {
                orientation = o[0];
                seg.querySelectorAll('button').forEach(function (x) {
                    x.classList.toggle('on', x.dataset.orient === orientation);
                });
                layout();
            };
            seg.appendChild(b);
        });
        seg.querySelector('button[data-orient="' + orientation + '"]').classList.add('on');
        orientSlot.appendChild(seg);
    }
    buildControls();

    // ---- mode --------------------------------------------------------
    function setMode(next) {
        if (next !== 'preview' && next !== 'editor') return;
        if (next === mode) return;
        mode = next;
        document.body.classList.toggle('mode-preview', mode === 'preview');
        if (mode === 'preview') {
            if (frame && !crashed) {
                // it kept running while the editor was in front: just show it
                layout();
            } else {
                // the editor may still owe the server the tail of a build — worth
                // a moment, never worth making the switch feel broken
                if (crashed) unmount();
                var drained = window.__o2DrainMirror ? window.__o2DrainMirror() : Promise.resolve();
                var bounded = Promise.race([drained, new Promise(function (r) { setTimeout(r, 1500); })]);
                bounded.then(function () {
                    if (mode !== 'preview') return;
                    mount();
                    layout();
                });
                layout();
            }
        }
        // leaving does not unload the client: it goes on running behind the
        // editor, the way the editor goes on running behind it
        var sw = document.getElementById('modeswitch');
        sw.classList.toggle('at-preview', mode === 'preview');
        sw.querySelectorAll('button').forEach(function (b) {
            b.classList.toggle('on', b.dataset.mode === mode);
        });
        emit('mode');
        // the editor canvas was hidden while away: let it size itself again
        window.dispatchEvent(new Event('resize'));
    }

    return {
        setMode: setMode,
        mode: function () { return mode; },
        isActive: function () { return mode === 'preview'; },
        isReady: function () { return mode === 'preview' && ready && !!frame; },
        // the client, whichever face is in front: it keeps running in the
        // background, so files changed meanwhile still have to reach its copy
        liveWindow: function () { return ready && frame ? frame.contentWindow : null; },
        frameWindow: function () { return frame && frame.contentWindow; },
        canvas: function () {
            try { return frame.contentDocument.getElementById('canvas'); } catch (e) { return null; }
        },
        restart: restart,
        device: function () {
            return { id: device.id, label: device.label, orientation: orientation,
                     width: screen.clientWidth, height: screen.clientHeight };
        },
        onChange: function (fn) { listeners.push(fn); },
    };
})();

(function () {
    document.querySelectorAll('#modeswitch button').forEach(function (b) {
        b.onclick = function () { o2Preview.setMode(b.dataset.mode); };
        b.classList.toggle('on', b.dataset.mode === 'editor');
    });
})();
