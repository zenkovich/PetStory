test = class test extends o2.Component
 {
    constructor()
     {
        super();

        this.a = 5;
        this.b = 'bla bla bla';
        this.c = false;
        this.d = { a: 5, b: false, c: 'stroka' };
        this.e = new Vec2(3, 50);
        this.f = new Rect(0, 1, 2, 3);
        this.g = new Border(0, 1, 2, 3);
        this.h = new Color4(255, 255, 255, 127);
        this.ref = new o2.RefAnimationAsset('xxx.anim');
        this.ref2= new o2.RefActorAsset();
        this.sprite = new o2.Sprite();
        this.actorRef = new o2.RefActor();
        this.bodyRef = new o2.RefRigidBody();
        this.curve = new o2.Curve();
        this.obj = new o2.EditorTestComponent.TestInside();
        this.array = [ 1, 2, 3, 4, 5 ];
        print('using fucking class');
    }

    OnStart() 
    {
        print('component started');
        print('actor name = ' + this._actor.name);

        print('children:')
        for (let child of this._actor.GetChildren())
            print(child.name)

        let act = new o2.Actor();
        act.name = 'from script';
        this._actor.AddChild(act);

        let fnd = this._actor.GetChild('Empty');
        this._actor.RemoveChild(fnd);
    }

    Update(dt)
     {
        this.a += dt;
    }
}