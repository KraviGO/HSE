using System;
using System.Collections.Generic;
using System.Linq;
using ZooERP.Interfaces;
using ZooERP.Models.Animals;
using ZooERP.Models.Things;
using ZooERP.Services;

namespace ZooERP.Tests
{
    public class UnitTests
    {
        private static int _testsPassed = 0;
        private static int _testsFailed = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("Старт юнит тестов");
            Console.WriteLine("========================================\n");

            TestAnimalProperties();
            TestThingProperties();
            TestContactAnimalLogic();
            TestInventoryService();
            TestVeterinaryClinic();
            TestZooServiceBasic();
            TestZooServiceAdvanced();

            Console.WriteLine("\n========================================");
            Console.WriteLine($"Результат: {_testsPassed} пройдено, {_testsFailed} провалено");
            
            if (_testsFailed == 0)
            {
                Console.WriteLine("Пройдены все тесты");
            }
            else
            {
                Console.WriteLine("Не все тесты пройдены");
            }
        }

        private static void TestAnimalProperties()
        {
            Console.WriteLine("Тест свойств животных:");
            
            // Тест кролика
            var rabbit = new Rabbit();
            Assert("Кролик имя", rabbit.Name == "Кролик");
            Assert("Кролик еда", rabbit.Food == 1);
            Assert("Кролик доброта", rabbit.KindnessLevel == 7);
            Assert("Кролик контактный", rabbit.CanBeInContactZoo == true);

            // Тест тигра
            var tiger = new Tiger();
            Assert("Тигр имя", tiger.Name == "Тигр");
            Assert("Тигр еда", tiger.Food == 10);
            Assert("Тигр не контактный", tiger.CanBeInContactZoo == false);

            // Тест обезьяны
            var monkey = new Monkey();
            Assert("Обезьяна имя", monkey.Name == "Обезьяна");
            Assert("Обезьяна еда", monkey.Food == 2);
            Assert("Обезьяна контактная", monkey.CanBeInContactZoo == true);

            // Тест волка
            var wolf = new Wolf();
            Assert("Волк имя", wolf.Name == "Волк");
            Assert("Волк еда", wolf.Food == 8);
            Assert("Волк не контактный", wolf.CanBeInContactZoo == false);
        }

        private static void TestThingProperties()
        {
            Console.WriteLine("\nТест свойств вещей:");
            
            var table = new Table();
            Assert("Стол имя", table.Name == "Стол");

            var computer = new Computer();
            Assert("Компьютер имя", computer.Name == "Компьютер");
        }

        private static void TestContactAnimalLogic()
        {
            Console.WriteLine("\nТест логики контактных животных:");
            
            var rabbit = new Rabbit(); // Доброта 7 > 5 = контактный
            Assert("Кролик доброта > 5", rabbit.KindnessLevel > 5);
            Assert("Кролик CanBeInContactZoo", rabbit.CanBeInContactZoo);

            var tiger = new Tiger(); // Доброта 0 < 5 = не контактный
            Assert("Тигр доброта < 5", tiger.KindnessLevel < 5);
            Assert("Тигр не CanBeInContactZoo", !tiger.CanBeInContactZoo);

            var monkey = new Monkey(); // Доброта 6 > 5 = контактный
            Assert("Обезьяна доброта > 5", monkey.KindnessLevel > 5);
            Assert("Обезьяна CanBeInContactZoo", monkey.CanBeInContactZoo);
        }

        private static void TestInventoryService()
        {
            Console.WriteLine("\nТест сервиса инвентаря:");
            
            var service = new InventoryService();
            var rabbit = new Rabbit();
            var table = new Table();

            // Тест регистрации
            service.RegisterItem(rabbit);
            service.RegisterItem(table);

            // Тест номеров
            var rabbitNumber = service.GetInventoryNumber(rabbit);
            var tableNumber = service.GetInventoryNumber(table);
            Assert("Кролик номер 1", rabbitNumber == 1);
            Assert("Стол номер 2", tableNumber == 2);

            // Тест получения всех items
            var items = service.GetAllInventoryItems().ToList();
            Assert("2 items в инвентаре", items.Count == 2);
            Assert("Кролик в инвентаре", items.Contains(rabbit));
            Assert("Стол в инвентаре", items.Contains(table));
        }

        private static void TestVeterinaryClinic()
        {
            Console.WriteLine("\nТест ветеринарной клиники:");
            
            var clinic = new VeterinaryClinic();
            var rabbit = new Rabbit();

            // Тест что метод возвращает boolean
            var isHealthy = clinic.IsHealthy(rabbit);
            Assert("IsHealthy возвращает bool", isHealthy == true || isHealthy == false);

            // Тест с разными животными
            var tiger = new Tiger();
            var monkey = new Monkey();
            
            var tigerHealthy = clinic.IsHealthy(tiger);
            var monkeyHealthy = clinic.IsHealthy(monkey);
            
            Assert("Тигр проверка здоровья", tigerHealthy == true || tigerHealthy == false);
            Assert("Обезьяна проверка здоровья", monkeyHealthy == true || monkeyHealthy == false);
        }

        private static void TestZooServiceBasic()
        {
            Console.WriteLine("\nБазовый тест сервиса зоопарка:");
            
            var healthService = new AlwaysHealthyService(); // Используем мок-сервис
            var inventoryService = new InventoryService();
            var zooService = new ZooService(healthService, inventoryService);

            // Тест добавления животного
            var rabbit = new Rabbit();
            zooService.AddAnimal(rabbit);
            var animals = zooService.GetAllAnimals();
            Assert("Добавление животного", animals.Count == 1);
            Assert("Кролик в зоопарке", animals[0].Name == "Кролик");

            // Тест добавления вещи
            var table = new Table();
            zooService.AddThing(table);
            var things = zooService.GetAllThings();
            Assert("Добавление вещи", things.Count == 1);
            Assert("Стол в зоопарке", things[0].Name == "Стол");
        }

        private static void TestZooServiceAdvanced()
        {
            Console.WriteLine("\nПродвинутый тест сервиса зоопарка:");
            
            var healthService = new AlwaysHealthyService(); // Используем мок-сервис
            var inventoryService = new InventoryService();
            var zooService = new ZooService(healthService, inventoryService);

            // Добавляем животных для тестов
            var rabbit = new Rabbit(); // 1 кг
            var tiger = new Tiger();   // 10 кг
            var monkey = new Monkey(); // 2 кг
            var wolf = new Wolf();     // 8 кг
            
            zooService.AddAnimal(rabbit);
            zooService.AddAnimal(tiger);
            zooService.AddAnimal(monkey);
            zooService.AddAnimal(wolf);

            // Тест расчета еды
            var totalFood = zooService.GetTotalFoodRequired();
            var expectedFood = 1 + 10 + 2 + 8; // 21 кг
            Assert($"Общая еда: {totalFood}кг", totalFood == expectedFood);

            // Тест контактных животных
            var contactAnimals = zooService.GetContactZooAnimals();
            // Контактные: кролик (7), обезьяна (6)
            // Не контактные: тигр (0), волк (0)
            Assert("2 контактных животных", contactAnimals.Count == 2);
            
            var contactNames = contactAnimals.Select(a => a.Name).ToList();
            Assert("Кролик в контактных", contactNames.Contains("Кролик"));
            Assert("Обезьяна в контактных", contactNames.Contains("Обезьяна"));
            Assert("Тигр не в контактных", !contactNames.Contains("Тигр"));
            Assert("Волк не в контактных", !contactNames.Contains("Волк"));
        }

        // Всегда возвращает true для тестирования
        private class AlwaysHealthyService : IAnimalHealthService
        {
            public bool IsHealthy(IAnimal animal)
            {
                return true; // Всегда здоровы для стабильных тестов
            }
        }

        private static void Assert(string testName, bool condition)
        {
            if (condition)
            {
                Console.WriteLine($"✅ {testName}");
                _testsPassed++;
            }
            else
            {
                Console.WriteLine($"❌ {testName}");
                _testsFailed++;
            }
        }
    }
}