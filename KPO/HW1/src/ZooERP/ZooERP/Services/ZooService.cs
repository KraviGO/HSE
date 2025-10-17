using ZooERP.Interfaces;

namespace ZooERP.Services
{
    public class ZooService : IZooService
    {
        private readonly List<IAnimal> _animals = new();
        private readonly List<IThing> _things = new();
        private readonly IAnimalHealthService _animalHealthService;
        private readonly IInventoryService _inventoryService;

        public ZooService(IAnimalHealthService animalHealthService, IInventoryService inventoryService)
        {
            _animalHealthService = animalHealthService;
            _inventoryService = inventoryService;
        }

        public void AddAnimal(IAnimal animal)
        {
            if (_animalHealthService.IsHealthy(animal))
            {
                _inventoryService.RegisterItem(animal);
                _animals.Add(animal);
                Console.WriteLine($"✅ {animal.Name} принят в зоопарк! Инвентарный №: {_inventoryService.GetInventoryNumber(animal)}");
            }
            else
            {
                Console.WriteLine($"❌ {animal.Name} не прошел проверку здоровья");
            }
        }

        public void AddThing(IThing thing)
        {
            _inventoryService.RegisterItem(thing);
            _things.Add(thing);
            Console.WriteLine($"✅ {thing.Name} добавлен! Инвентарный №: {_inventoryService.GetInventoryNumber(thing)}");
        }

        public int GetTotalFoodRequired()
        {
            var totalFood = _animals.Sum(a => a.Food);
            Console.WriteLine($"\nОбщее количество еды: {totalFood} кг/день");
            return totalFood;
        }

        public List<IAnimal> GetContactZooAnimals()
        {
            return _animals
                .Where(animal => animal is IContactAnimal contactAnimal && contactAnimal.CanBeInContactZoo)
                .ToList();
        }
        
        public int GetInventoryNumber(INamedItem item)
        {
            return _inventoryService.GetInventoryNumber(item);
        }

        public void PrintInventoryReport()
        {
            Console.WriteLine("\n" + new string('=', 50));
            Console.WriteLine("Отчет по инвентарю зоопарка");
            Console.WriteLine(new string('=', 50));

            Console.WriteLine("\nживотные:");
            foreach (var animal in _animals)
            {
                Console.WriteLine($"{animal.Name} - №{_inventoryService.GetInventoryNumber(animal)} - {animal.Food} кг/день");
            }

            Console.WriteLine("\nвещи:");
            foreach (var thing in _things)
            {
                Console.WriteLine($"{thing.Name} - №{_inventoryService.GetInventoryNumber(thing)}");
            }

            Console.WriteLine($"\nитого: {_animals.Count} животных, {_things.Count} вещей");
        }

        public List<IAnimal> GetAllAnimals() => _animals;
        public List<IThing> GetAllThings() => _things;
    }
}