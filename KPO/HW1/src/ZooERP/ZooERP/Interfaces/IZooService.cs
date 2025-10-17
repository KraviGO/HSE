namespace ZooERP.Interfaces
{
    public interface IZooService
    {
        void AddAnimal(IAnimal animal);
        void AddThing(IThing thing);
        int GetTotalFoodRequired();
        List<IAnimal> GetContactZooAnimals();
        void PrintInventoryReport();
        List<IAnimal> GetAllAnimals();
        List<IThing> GetAllThings();
        int GetInventoryNumber(INamedItem item);
    }
}