namespace ZooERP.Interfaces
{
    public interface IAnimalHealthService
    {
        bool IsHealthy(IAnimal livingEntity);
    }
}