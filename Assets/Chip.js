Chip = class Chip extends o2.Component
{
    constructor()
    {
        super();

        this.type = "yellow";
        this.groupingRadius = 100;
        this.image = new o2.RefImageComponent();

        print('chip create');
    }

    OnStart()
    {
        this.image = this._actor.GetComponent("o2::ImageComponent");
    }
}