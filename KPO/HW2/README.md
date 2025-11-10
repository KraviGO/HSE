**ФИО: Кравченко Игорь Александрович**

**Группа: БПИ249**

**Telegram: @KraviGO**



## **1. Клонировать репозиторий**

```
git clone https://github.com/KraviGO/...
cd FinanceTracker
```



## **2. Запуск Backend (API)**

**Путь:** FinanceTracker/Finance.Api

### **Команды:**

```
dotnet restore
dotnet ef database update
dotnet run
```

**API запустится по адресу:**  http://localhost:5182/swagger



## **3. Запуск Frontend (React)**

**Путь:** finance-client

### **Команды:**

```
npm install
npm run dev
```

**Фронт запустится по адресу:**  http://localhost:5173/



## **4. Консольная версия**

**Путь:** FinanceTracker/Finance.ConsoleApp

### **Команда:**

```
dotnet run
```

Будет запущен консольный интерфейс с навигацией по меню и теми же функциями, что и в API.



## **Содержимое слоёв**

| **Слой**           | **Назначение**                                               |
| ------------------ | ------------------------------------------------------------ |
| **Domain**         | Сущности (User, Operation, BankAccount, Goal) и интерфейсы репозиториев (Ports) |
| **Application**    | Сервисы, аналитика, бизнес-логика, не зависящая от БД        |
| **Infrastructure** | Реализация интерфейсов, база данных (EF Core + SQLite), импортеры и экспортеры |
| **Api**            | HTTP endpoints и конфигурация приложения                     |
| **ConsoleApp**     | CLI-версия с теми же сервисами, без веб-интерфейса           |
| **finance-client** | React-фронтенд с Material UI, React Query и Vite             |



## **Используемые технологии**

| **Категория**   | **Технологии**                                      |
| --------------- | --------------------------------------------------- |
| Backend         | .NET 8, ASP.NET Core, Entity Framework Core, SQLite |
| Frontend        | React 18, Vite, Material UI (MUI), React Query      |
| Хранение данных | SQLite (локальный файл finance.db)                  |



## **Ключевые принципы**

- **Domain не зависит ни от чего.**

  Только чистые классы и интерфейсы (никакого EF или UI).

- **Application использует только Domain.**

  Логика сервисов и аналитики, не знает о БД или инфраструктуре.

- **Infrastructure реализует интерфейсы из Domain.**

  Здесь вся работа с EF, файлами, импортом/экспортом и т.п.

- **Api и ConsoleApp используют Application.**

  Но не содержат бизнес-логики — только “входные точки”.