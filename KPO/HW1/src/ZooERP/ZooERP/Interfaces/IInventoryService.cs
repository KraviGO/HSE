namespace ZooERP.Interfaces
{
    public interface IInventoryService
    {
        void RegisterItem(INamedItem item);
        int GetInventoryNumber(INamedItem item);
        IEnumerable<INamedItem> GetAllInventoryItems();
    }
}