using Microsoft.Extensions.DependencyInjection;
using ZooERP.Interfaces;
using ZooERP.Models.Animals;
using ZooERP.Models.Things;
using ZooERP.Services;
using ZooERP.Tests;

class Program
{
    static void Main()
    {
        UnitTests.RunAllTests();
        
        var services = new ServiceCollection();
        services.AddTransient<IAnimalHealthService, VeterinaryClinic>();
        services.AddSingleton<IInventoryService, InventoryService>();
        services.AddSingleton<IZooService, ZooService>();
        
        var serviceProvider = services.BuildServiceProvider();
        var zooService = serviceProvider.GetRequiredService<IZooService>();
        
        Console.WriteLine("\nДобавление животных:");
        zooService.AddAnimal(new Rabbit());
        zooService.AddAnimal(new Tiger());
        zooService.AddAnimal(new Monkey());
        zooService.AddAnimal(new Wolf());
        zooService.AddAnimal(new Rabbit());
        
        Console.WriteLine("\nДобавление вещей:");
        zooService.AddThing(new Table());
        zooService.AddThing(new Computer());
        zooService.AddThing(new Table());
        
        zooService.GetTotalFoodRequired();
        
        var contactAnimals = zooService.GetContactZooAnimals();
        Console.WriteLine($"\nЖивотные для контактного зоопарка ({contactAnimals.Count}):");
        foreach (var animal in contactAnimals)
        {
            var contactAnimal = animal as IContactAnimal;
            Console.WriteLine($"  {animal.Name} (уровень доброты: {contactAnimal?.KindnessLevel}/10) - №{zooService.GetInventoryNumber(animal)}");
        }
        
        zooService.PrintInventoryReport();
    }
}