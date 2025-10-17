using ZooERP.Interfaces;
using ZooERP.Models.Animals;

namespace ZooERP.Services
{
    public class VeterinaryClinic : IAnimalHealthService
    {
        public bool IsHealthy(IAnimal animal)
        {
            var random = new Random();
            var isHealthy = random.Next(0, 10) < 8;

            Console.WriteLine($"Ветеринарная проверка {animal.Name}: {(isHealthy ? "здоров" : "не здоров")}");
            return isHealthy;
        }
    }
}