using ZooERP.Interfaces;

namespace ZooERP.Models.Animals
{
    public abstract class Animal : IAnimal
    {
        public string Name { get; set; }
        public abstract int Food { get; }
        public abstract int KindnessLevel { get; }
        public bool CanBeInContactZoo => KindnessLevel > 5;
    }
}