//let test = function() { print('fuck you'); };

test = class test {
    constructor() {
        // this.a = 5;
        // this.b = 'bla bla bla';
        // this.c = false;
        // this.d = { a: 5, b: false, c: 'stroka' };
        // this.e = new Vec2(3, 50);
        // this.f = new Rect(0, 1, 2, 3);
        // this.g = new Border(0, 1, 2, 3);
        // this.h = new Color4(255, 255, 255, 127);
        // this.ref = o2.RefAnimationAsset.New('xxx.anim');
        // this.ref2= o2.RefActorAsset.New();
        // this.sprite = o2.Sprite.New();
        // this.actorRef = o2.RefActor.New();
        // this.bodyRef = o2.RefRigidBody.New();
        // this.curve = o2.Curve.New();
        this.obj = o2.EditorTestComponent.TestInside.New();
        print(JSON.stringify(this.obj));
        this.obj.mFloat = 12;
        print(JSON.stringify(this.obj));
        this.array = [ 1, 2, 3, 4, 5 ];
        print('using fucking class');
    }

    OnStart() {
        print('component started');
    }

    Update(dt) {
        this.a += dt;
        print('this is ' + JSON.stringify(this));
    }
}