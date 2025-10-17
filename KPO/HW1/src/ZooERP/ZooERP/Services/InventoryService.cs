using ZooERP.Interfaces;

namespace ZooERP.Services
{
    public class InventoryService : IInventoryService
    {
        private int _nextNumber = 1;
        private readonly Dictionary<INamedItem, int> _inventoryNumbers = new();

        public void RegisterItem(INamedItem item)
        {
            _inventoryNumbers[item] = _nextNumber++;
        }

        public int GetInventoryNumber(INamedItem item)
        {
            return _inventoryNumbers[item];
        }

        public IEnumerable<INamedItem> GetAllInventoryItems()
        {
            return _inventoryNumbers.Keys;
        }
    }
}