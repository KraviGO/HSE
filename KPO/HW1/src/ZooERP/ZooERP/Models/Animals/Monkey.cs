namespace ZooERP.Models.Animals
{
    public class Monkey : Animal
    {
        public override int Food => 2;
        public override int KindnessLevel => 6;

        public Monkey()
        {
            Name = "Обезьяна";
        }
    }
}