namespace ZooERP.Interfaces
{
    public interface IContactAnimal
    {
        int KindnessLevel { get; }
        bool CanBeInContactZoo { get; }
    }
}