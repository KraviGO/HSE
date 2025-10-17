namespace ZooERP.Models.Animals
{
    public class Wolf : Predator
    {
        public override int Food => 8;

        public Wolf()
        {
            Name = "Волк";
        }
    }
}