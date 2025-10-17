using ZooERP.Interfaces;

namespace ZooERP.Models.Things
{
    public abstract class Thing : IThing
    {
        public abstract string Name { get; }
    }
}