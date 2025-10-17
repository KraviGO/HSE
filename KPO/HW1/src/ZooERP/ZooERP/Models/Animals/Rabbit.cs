namespace ZooERP.Models.Animals
{
    public class Rabbit : Herbo
    {
        public override int Food => 1;
        public override int KindnessLevel => 7;

        public Rabbit()
        {
            Name = "Кролик";
        }
    }
}