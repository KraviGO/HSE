namespace ZooERP.Models.Animals
{
    public class Tiger : Predator
    {
        public override int Food => 10;

        public Tiger()
        {
            Name = "Тигр";
        }
    }
}