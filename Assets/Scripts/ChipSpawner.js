ChipSpawner = class ChipSpawner extends o2.Component
{
    constructor()
    {
        super();

        this.spawnDelay = 0.2;  
        this.maxChipsCount = 20; 
        this.spawnContainer = new o2.RefActor();     
        this.spawnZone = new o2.RefActor();          
        this.chipProto = new o2.RefActorAsset();          
    
        this._accumulatedTimer = 0.0;
    }

    Update(dt)
    {
        this._accumulatedTimer += dt;
        if (this._accumulatedTimer > this.spawnDelay)
        {
            this._accumulatedTimer -= this.spawnDelay;
            this.CheckChipsCount();
        }
    }

    CheckChipsCount()
    {        
        let currentCount = this.spawnContainer.Get().GetChildren()
            .filter(actor => actor.GetPrototype() == this.chipProto).length;
            
        if (currentCount >= this.maxChipsCount)
            return;

        let spawnZone = this.spawnZone.Get().transform.worldRect;
        
        print(Dump(this.chipProto.Get()));
        let newChip = this.chipProto.Get().Instantiate();
        // newChip.transform.position = new Vec2(Math.randomBetween(spawnZone.left, spawnZone.right),
        //                                       Math.randomBetween(spawnZone.bottom, spawnZone.top));
                                           
        // newChip.SetParent(this.spawnContainer.Get());
    }
}