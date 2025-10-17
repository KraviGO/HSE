namespace ZooERP.Models.Animals
{
    public abstract class Predator : Animal
    {
        public override int KindnessLevel => 0;
    }
}