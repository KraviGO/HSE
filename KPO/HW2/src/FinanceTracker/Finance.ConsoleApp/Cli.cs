using Finance.Domain.Entities;
using Spectre.Console;
using Finance.Application.Services;
using Finance.Domain.Ports.Repositories;
using Finance.Domain.Ports.Services;
using IAccountRepository = Finance.Domain.Ports.Repositories.IAccountRepository;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

public class Cli
{
    private readonly IUserRepository _users;
    private readonly IAccountRepository _accounts;
    private readonly ICategoryRepository _categories;
    private readonly IOperationRepository _operations;
    private readonly IImportService _import;
    private readonly IExportVisitor _export;
    private readonly IGoalRepository _goals;
    private readonly TransferService _transfer;

    private User? _currentUser;

    public Cli(
        IUserRepository users,
        IAccountRepository accounts,
        ICategoryRepository categories,
        IOperationRepository operations,
        IImportService import,
        IExportVisitor export,
        IGoalRepository goals,
        TransferService transfer)
    {
        _users = users;
        _accounts = accounts;
        _categories = categories;
        _operations = operations;
        _import = import;
        _export = export;
        _goals = goals;
        _transfer = transfer;
    }

    public async Task RunAsync()
    {
        while (true)
        {
            var items = new List<string>
            {
                _currentUser == null
                    ? "👤 Выбрать/создать пользователя"
                    : $"👤 Текущий: {Markup.Escape(_currentUser.Name)} ({Markup.Escape(_currentUser.Email)})",
                "💳 Счета",
                "💸 Операции",
                "🏷 Категории",
                "🔁 Переводы между счетами",
                "🎯 Финансовые цели",
                "📊 Аналитика",
                "📂 Импорт/Экспорт",
                "🚪 Выход"
            };

            var choice = AnsiConsole.Prompt(
                new SelectionPrompt<string>()
                    .Title("[bold cyan]Главное меню[/]")
                    .HighlightStyle(new Style(foreground: Color.Yellow))
                    .AddChoices(items)
            );

            if (choice.StartsWith("🚪"))
                return;

            if (choice.StartsWith("👤"))
                await UserMenuAsync();
            else if (choice.StartsWith("💳"))
                await AccountMenuAsync();
            else if (choice.StartsWith("💸"))
                await OperationMenuAsync();
            else if (choice.StartsWith("🏷"))
                await CategoryMenuAsync();
            else if (choice.StartsWith("🔁"))
                await TransferMenuAsync();
            else if (choice.StartsWith("🎯"))
                await GoalsMenuAsync();
            else if (choice.StartsWith("📊"))
                await AnalyticsMenuAsync();
            else if (choice.StartsWith("📂"))
                await ImportExportMenuAsync();
        }
    }

    // ---------- Users ----------
    private async Task UserMenuAsync()
    {
        var action = AnsiConsole.Prompt(
            new SelectionPrompt<string>()
                .Title("[bold cyan]Пользователи[/]")
                .AddChoices("➕ Создать", "🔁 Переключить", "⬅️ Назад")
        );

        if (action.StartsWith("⬅️")) return;

        if (action.StartsWith("➕"))
        {
            var name = AnsiConsole.Ask<string>("Имя:");
            var email = AnsiConsole.Ask<string>("Email:");

            var user = new User { Name = name, Email = email, CreatedAt = DateTime.UtcNow };
            await _users.AddAsync(user);
            await _users.SaveChangesAsync();
            _currentUser = user;

            AnsiConsole.MarkupLine("[green]✅ Пользователь создан и выбран[/]");
            return;
        }

        var all = (await _users.GetAllAsync()).ToList();
        if (all.Count == 0)
        {
            AnsiConsole.MarkupLine("[yellow]Нет пользователей[/]");
            return;
        }

        var selected = AnsiConsole.Prompt(
            new SelectionPrompt<User>()
                .Title("Выберите пользователя")
                .UseConverter(u => Markup.Escape($"{u.Name} <{u.Email}>"))
                .AddChoices(all));

        _currentUser = selected;
        AnsiConsole.MarkupLine($"[green]✅ Выбран: {Markup.Escape(_currentUser.Name)}[/]");
    }

    // ---------- Accounts ----------
    private async Task AccountMenuAsync()
    {
        if (!EnsureUser()) return;

        var action = AnsiConsole.Prompt(
            new SelectionPrompt<string>()
                .Title("[bold cyan]💳 Счета[/]")
                .AddChoices("📋 Список", "➕ Создать", "✏️ Переименовать", "🗑 Удалить", "⬅️ Назад")
        );

        if (action.StartsWith("⬅️")) return;

        if (action.StartsWith("📋"))
        {
            var list = await _accounts.GetByUserAsync(_currentUser!.Id);
            var table = new Table().Border(TableBorder.Rounded).Title("[bold]Ваши счета[/]");
            table.AddColumn("Название");
            table.AddColumn("Баланс");
            table.AddColumn("Валюта");

            foreach (var a in list)
                table.AddRow(Markup.Escape(a.Name), $"{a.Balance:0.00}", Markup.Escape(a.Currency));

            AnsiConsole.Write(table);
            return;
        }

        if (action.StartsWith("➕"))
        {
            var name = AnsiConsole.Ask<string>("Название счета:");
            var currency = AnsiConsole.Ask<string>("Валюта (например, RUB):", "RUB");
            var acc = new BankAccount
            {
                UserId = _currentUser!.Id,
                Name = name,
                Currency = currency,
                Balance = 0m,
                CreatedAt = DateTime.UtcNow
            };
            await _accounts.AddAsync(acc);
            await _accounts.SaveChangesAsync();
            AnsiConsole.MarkupLine("[green]✅ Счет создан[/]");
            return;
        }

        var accounts = (await _accounts.GetByUserAsync(_currentUser!.Id)).ToList();
        if (accounts.Count == 0)
        {
            AnsiConsole.MarkupLine("[yellow]⚠️ У пользователя нет счетов[/]");
            return;
        }

        var pick = AnsiConsole.Prompt(
            new SelectionPrompt<BankAccount>()
                .Title("Выберите счет")
                .UseConverter(a => Markup.Escape($"{a.Name} — {a.Balance:0.00} {a.Currency}"))
                .AddChoices(accounts));

        if (action.StartsWith("✏️"))
        {
            var newName = AnsiConsole.Ask<string>("Новое имя:", pick.Name);
            pick.Name = newName;
            await _accounts.UpdateAsync(pick);
            await _accounts.SaveChangesAsync();
            AnsiConsole.MarkupLine("[green]✅ Счет обновлен[/]");
        }
        else if (action.StartsWith("🗑"))
        {
            await _accounts.DeleteAsync(pick.Id);
            await _accounts.SaveChangesAsync();
            AnsiConsole.MarkupLine("[green]🗑 Счет удален[/]");
        }
    }

    // ---------- Operations ----------
    private async Task OperationMenuAsync()
    {
        if (!EnsureUser()) return;

        var action = AnsiConsole.Prompt(
            new SelectionPrompt<string>()
                .Title("[bold cyan]Операции[/]")
                .AddChoices("📋 Список", "➕ Добавить", "✏️ Изменить", "🗑 Удалить", "⬅️ Назад")
        );

        if (action.StartsWith("⬅️")) return;

        if (action.StartsWith("📋"))
        {
            await ShowOperationsAsync();
            return;
        }

        if (action.StartsWith("➕"))
        {
            var op = await BuildOperationAsync();
            if (op == null) return;

            var acc = await _accounts.GetByIdAsync(op.AccountId);
            if (acc == null) { AnsiConsole.MarkupLine("[red]❌ Счет не найден[/]"); return; }
            acc.Balance += op.Amount;

            await _operations.AddAsync(op);
            await _accounts.UpdateAsync(acc);
            await _operations.SaveChangesAsync();
            await _accounts.SaveChangesAsync();

            AnsiConsole.MarkupLine("[green]✅ Операция добавлена[/]");
            return;
        }

        var ops = (await _operations.GetByUserAsync(_currentUser!.Id)).ToList();
        if (ops.Count == 0)
        {
            AnsiConsole.MarkupLine("[yellow]⚠️ Операций нет[/]");
            return;
        }

        var pick = AnsiConsole.Prompt(
            new SelectionPrompt<Operation>()
                .Title("Выберите операцию")
                .UseConverter(o => Markup.Escape($"{o.OccurredOn}: {o.Amount:0.00} {(o.Category?.Name ?? "Без категории")} [{o.Description}]"))
                .AddChoices(ops));

        if (action.StartsWith("✏️"))
        {
            var newAmount = AnsiConsole.Ask<decimal>("Сумма:", pick.Amount);
            var diff = newAmount - pick.Amount;
            var newDesc = AnsiConsole.Ask<string>("Описание:", pick.Description ?? "");

            pick.Amount = newAmount;
            pick.Description = newDesc;

            var acc = await _accounts.GetByIdAsync(pick.AccountId);
            if (acc != null)
            {
                acc.Balance += diff;
                await _accounts.UpdateAsync(acc);
                await _accounts.SaveChangesAsync();
            }

            await _operations.UpdateAsync(pick);
            await _operations.SaveChangesAsync();

            AnsiConsole.MarkupLine("[green]✅ Операция обновлена[/]");
        }
        else if (action.StartsWith("🗑"))
        {
            var acc = await _accounts.GetByIdAsync(pick.AccountId);
            if (acc != null)
            {
                acc.Balance -= pick.Amount;
                await _accounts.UpdateAsync(acc);
                await _accounts.SaveChangesAsync();
            }

            await _operations.DeleteAsync(pick.Id);
            await _operations.SaveChangesAsync();

            AnsiConsole.MarkupLine("[green]🗑 Операция удалена[/]");
        }
    }

    private async Task ShowOperationsAsync()
    {
        var list = await _operations.GetByUserAsync(_currentUser!.Id);
        var table = new Table().Border(TableBorder.Rounded).Title("[bold]📋 Операции[/]");
        table.AddColumn("Дата");
        table.AddColumn("Счет");
        table.AddColumn("Категория");
        table.AddColumn("Сумма");
        table.AddColumn("Описание");

        foreach (var o in list)
        {
            table.AddRow(
                o.OccurredOn.ToString("yyyy-MM-dd"),
                Markup.Escape(o.Account?.Name ?? "-"),
                Markup.Escape(o.Category?.Name ?? "-"),
                $"{o.Amount:0.00}",
                Markup.Escape(o.Description ?? "-")
            );
        }

        AnsiConsole.Write(table);
    }

    private async Task<Operation?> BuildOperationAsync()
    {
        var accounts = (await _accounts.GetByUserAsync(_currentUser!.Id)).ToList();
        if (accounts.Count == 0)
        {
            AnsiConsole.MarkupLine("[yellow]⚠️ Сначала создайте счет[/]");
            return null;
        }

        var acc = AnsiConsole.Prompt(
            new SelectionPrompt<BankAccount>()
                .Title("Счет:")
                .UseConverter(a => Markup.Escape($"{a.Name} — {a.Balance:0.00} {a.Currency}"))
                .AddChoices(accounts));

        var categories = (await _categories.GetByUserAsync(_currentUser!.Id)).ToList();
        Category? cat = null;
        if (categories.Count > 0)
        {
            var pick = AnsiConsole.Prompt(
                new SelectionPrompt<string>()
                    .Title("Категория:")
                    .AddChoices(categories.Select(c => Markup.Escape(c.Name)).Append("Без категории")));
            if (pick != "Без категории")
                cat = categories.First(c => c.Name == pick);
        }

        var isExpense = AnsiConsole.Confirm("Это расход?");
        var sum = AnsiConsole.Ask<decimal>("Сумма:");
        if (isExpense) sum = -Math.Abs(sum);

        var when = AnsiConsole.Ask<DateTime>("Дата (yyyy-MM-dd):", DateTime.UtcNow);
        var desc = AnsiConsole.Ask<string>("Описание (optional):", "");

        return new Operation
        {
            UserId = _currentUser!.Id,
            AccountId = acc.Id,
            CategoryId = cat?.Id,
            Amount = sum,
            OccurredOn = DateOnly.FromDateTime(when),
            Description = string.IsNullOrWhiteSpace(desc) ? null : desc
        };
    }
    
    // ---------- Categories ----------
    private async Task CategoryMenuAsync()
    {
        if (!EnsureUser()) return;

        while (true)
        {
            var action = AnsiConsole.Prompt(
                new SelectionPrompt<string>()
                    .Title("[bold cyan]🏷 Категории[/]")
                    .AddChoices("📋 Список", "➕ Создать", "✏️ Изменить", "🗑 Удалить", "⬅️ Назад")
            );

            if (action.StartsWith("⬅️")) return;

            if (action.StartsWith("📋"))
            {
                var list = await _categories.GetByUserAsync(_currentUser!.Id);
                if (!list.Any())
                {
                    AnsiConsole.MarkupLine("[gray]Категорий нет[/]");
                    continue;
                }

                var table = new Table().Border(TableBorder.Rounded).Title("[bold]🏷 Категории[/]");
                table.AddColumn("Название");
                table.AddColumn("Тип");

                foreach (var c in list)
                    table.AddRow(Markup.Escape(c.Name), c.IsIncome ? "Доход" : "Расход");

                AnsiConsole.Write(table);
            }
            else if (action.StartsWith("➕"))
            {
                var name = AnsiConsole.Ask<string>("Название категории:");
                var isIncome = AnsiConsole.Confirm("Это категория дохода?");
                var category = new Category
                {
                    UserId = _currentUser!.Id,
                    Name = name,
                    IsIncome = isIncome
                };

                await _categories.AddAsync(category);
                await _categories.SaveChangesAsync();

                AnsiConsole.MarkupLine("[green]✅ Категория добавлена[/]");
            }
            else if (action.StartsWith("✏️"))
            {
                var list = await _categories.GetByUserAsync(_currentUser!.Id);
                if (!list.Any())
                {
                    AnsiConsole.MarkupLine("[gray]Нет категорий для изменения[/]");
                    continue;
                }

                var pick = AnsiConsole.Prompt(
                    new SelectionPrompt<Category>()
                        .Title("Выберите категорию:")
                        .UseConverter(c => $"{c.Name} ({(c.IsIncome ? "Доход" : "Расход")})")
                        .AddChoices(list));

                var newName = AnsiConsole.Ask<string>("Новое имя категории:", pick.Name);
                var newType = AnsiConsole.Confirm("Это категория дохода?", pick.IsIncome);

                pick.Name = newName;
                pick.IsIncome = newType;

                await _categories.UpdateAsync(pick);
                await _categories.SaveChangesAsync();

                AnsiConsole.MarkupLine("[green]✅ Категория обновлена[/]");
            }
            else if (action.StartsWith("🗑"))
            {
                var list = await _categories.GetByUserAsync(_currentUser!.Id);
                if (!list.Any())
                {
                    AnsiConsole.MarkupLine("[gray]Нет категорий для удаления[/]");
                    continue;
                }

                var pick = AnsiConsole.Prompt(
                    new SelectionPrompt<Category>()
                        .Title("Выберите категорию для удаления:")
                        .UseConverter(c => $"{c.Name} ({(c.IsIncome ? "Доход" : "Расход")})")
                        .AddChoices(list));

                if (AnsiConsole.Confirm($"Удалить категорию [red]{Markup.Escape(pick.Name)}[/]?"))
                {
                    await _categories.DeleteAsync(pick.Id);
                    await _categories.SaveChangesAsync();
                    AnsiConsole.MarkupLine("[green]🗑 Категория удалена[/]");
                }
            }
        }
    }
    
    private async Task TransferMenuAsync()
    {
        if (!EnsureUser()) return;

        var accounts = (await _accounts.GetByUserAsync(_currentUser!.Id)).ToList();
        if (accounts.Count < 2)
        {
            AnsiConsole.MarkupLine("[yellow]⚠️ Нужно как минимум два счёта для перевода.[/]");
            return;
        }

        var from = AnsiConsole.Prompt(
            new SelectionPrompt<BankAccount>()
                .Title("С какого счёта перевести?")
                .UseConverter(a => $"{a.Name} — {a.Balance:0.00} {a.Currency}")
                .AddChoices(accounts));

        var to = AnsiConsole.Prompt(
            new SelectionPrompt<BankAccount>()
                .Title("На какой счёт перевести?")
                .UseConverter(a => $"{a.Name} — {a.Balance:0.00} {a.Currency}")
                .AddChoices(accounts.Where(a => a.Id != from.Id)));

        decimal amount;

        while (true)
        {
            var input = AnsiConsole.Ask<string>(
                "Введите сумму перевода ([grey]0 или q для отмены[/]):");

            if (string.Equals(input, "q", StringComparison.OrdinalIgnoreCase))
            {
                AnsiConsole.MarkupLine("[yellow]🚫 Перевод отменён пользователем[/]");
                return;
            }

            if (!decimal.TryParse(input, out amount))
            {
                AnsiConsole.MarkupLine("[red]❌ Неверный формат числа. Попробуйте снова.[/]");
                continue;
            }

            if (amount <= 0)
            {
                AnsiConsole.MarkupLine("[yellow]🚫 Операция отменена или сумма должна быть > 0[/]");
                return;
            }

            if (amount > from.Balance)
            {
                AnsiConsole.MarkupLine("[red]Недостаточно средств на счёте-источнике.[/]");
                continue;
            }

            break;
        }

        try
        {
            await _transfer.TransferAsync(from.Id, to.Id, amount, _currentUser!.Id, null, "Перевод между счетами");
            AnsiConsole.MarkupLine($"[green]✅ Переведено {amount:0.00} {from.Currency} с {from.Name} на {to.Name}[/]");
        }
        catch (Exception ex)
        {
            AnsiConsole.MarkupLine($"[red]❌ Ошибка перевода: {Markup.Escape(ex.Message)}[/]");
        }
    }
    
    private async Task GoalsMenuAsync()
    {
        if (!EnsureUser()) return;

        while (true)
        {
            var action = AnsiConsole.Prompt(
                new SelectionPrompt<string>()
                    .Title("[bold cyan]🎯 Финансовые цели[/]")
                    .AddChoices("📋 Список целей", "➕ Создать новую", "💰 Пополнить цель", "🗑 Удалить", "⬅️ Назад"));

            if (action.StartsWith("⬅️")) break;

            if (action.StartsWith("📋"))
            {
                var goals = (await _goals.GetByUserAsync(_currentUser!.Id)).ToList();
                if (!goals.Any())
                {
                    AnsiConsole.MarkupLine("[yellow]Пока нет активных целей[/]");
                    continue;
                }

                var table = new Table().Border(TableBorder.Rounded).Title("[bold]Ваши цели[/]");
                table.AddColumn("Название");
                table.AddColumn("Целевой счёт");
                table.AddColumn("Текущая сумма");
                table.AddColumn("Цель");
                table.AddColumn("Прогресс");
                table.AddColumn("Дедлайн");

                foreach (var g in goals)
                {
                    var acc = await _accounts.GetByIdAsync(g.AccountId);
                    var color = g.IsAchieved ? "green" : "yellow";
                    table.AddRow(
                        Markup.Escape(g.Name),
                        Markup.Escape(acc?.Name ?? "-"),
                        $"{acc?.Balance ?? 0:0.00}",
                        $"{g.TargetAmount:0.00}",
                        $"[{color}]{g.Progress:0.0}%[/]",
                        g.Deadline.ToString("dd.MM.yyyy"));
                }

                AnsiConsole.Write(table);
            }
            else if (action.StartsWith("➕"))
            {
                var name = AnsiConsole.Ask<string>("Название цели:");
                var target = AnsiConsole.Ask<decimal>("Целевая сумма:");
                var deadline = AnsiConsole.Ask<DateTime>("Дедлайн (yyyy-MM-dd):", DateTime.Now.AddMonths(1));

                var goalAccount = new BankAccount
                {
                    UserId = _currentUser!.Id,
                    Name = $"Цель: {name}",
                    Currency = "RUB",
                    Balance = 0m,
                    CreatedAt = DateTime.UtcNow
                };
                await _accounts.AddAsync(goalAccount);
                await _accounts.SaveChangesAsync();

                var goal = new FinancialGoal
                {
                    UserId = _currentUser!.Id,
                    AccountId = goalAccount.Id,
                    Name = name,
                    TargetAmount = target,
                    Deadline = DateOnly.FromDateTime(deadline),
                    CurrentAmount = 0
                };

                await _goals.AddAsync(goal);
                await _goals.SaveChangesAsync();
                AnsiConsole.MarkupLine($"[green]✅ Цель '{name}' создана, создан счёт {goalAccount.Name}[/]");
            }
            else if (action.StartsWith("💰"))
            {
                var goals = (await _goals.GetByUserAsync(_currentUser!.Id)).ToList();
                if (!goals.Any()) { AnsiConsole.MarkupLine("[yellow]Нет целей[/]"); continue; }

                var goal = AnsiConsole.Prompt(
                    new SelectionPrompt<FinancialGoal>()
                        .Title("Выберите цель для пополнения")
                        .UseConverter(g => $"{g.Name} ({g.Progress:0.0}% — {g.CurrentAmount:0.00}/{g.TargetAmount:0.00})")
                        .AddChoices(goals));

                var accounts = (await _accounts.GetByUserAsync(_currentUser!.Id))
                    .Where(a => a.Id != goal.AccountId).ToList();

                if (!accounts.Any()) { AnsiConsole.MarkupLine("[yellow]Нет других счетов для перевода[/]"); continue; }

                var from = AnsiConsole.Prompt(
                    new SelectionPrompt<BankAccount>()
                        .Title("С какого счёта перевести?")
                        .UseConverter(a => $"{a.Name} ({a.Balance:0.00} {a.Currency})")
                        .AddChoices(accounts));

                var amount = AnsiConsole.Ask<decimal>("Сумма перевода:");
                
                if (amount <= 0)
                {
                    AnsiConsole.MarkupLine("[red]Сумма должна быть положительной и больше нуля.[/]");
                    continue;
                }
                
                try
                {
                    await _transfer.TransferAsync(from.Id, goal.AccountId, amount, _currentUser!.Id, null, $"Пополнение цели: {goal.Name}");
                    AnsiConsole.MarkupLine($"[green]✅ Цель '{goal.Name}' пополнена на {amount:0.00} ₽[/]");
                }
                catch (Exception ex)
                {
                    AnsiConsole.MarkupLine($"[red]❌ Ошибка: {Markup.Escape(ex.Message)}[/]");
                }
            }
            else if (action.StartsWith("🗑"))
            {
                var goals = (await _goals.GetByUserAsync(_currentUser!.Id)).ToList();
                if (!goals.Any()) { AnsiConsole.MarkupLine("[yellow]Нет целей[/]"); continue; }

                var goal = AnsiConsole.Prompt(
                    new SelectionPrompt<FinancialGoal>()
                        .Title("Выберите цель для удаления")
                        .UseConverter(g => g.Name)
                        .AddChoices(goals));

                await _goals.DeleteAsync(goal.Id);
                await _goals.SaveChangesAsync();

                var acc = await _accounts.GetByIdAsync(goal.AccountId);
                if (acc != null)
                {
                    await _accounts.DeleteAsync(acc.Id);
                    await _accounts.SaveChangesAsync();
                }

                AnsiConsole.MarkupLine("[green]🗑 Цель удалена вместе со счётом[/]");
            }
        }
    }

    private async Task AnalyticsMenuAsync()
    {
        if (!EnsureUser()) return;

        var ops = (await _operations.GetByUserAsync(_currentUser!.Id)).ToList();
        if (ops.Count == 0)
        {
            AnsiConsole.MarkupLine("[yellow]⚠️ У этого пользователя пока нет операций[/]");
            return;
        }

        // --- Общая сводка ---
        var totalIncome = ops.Where(o => o.Amount > 0).Sum(o => o.Amount);
        var totalExpense = ops.Where(o => o.Amount < 0).Sum(o => o.Amount);
        var net = totalIncome + totalExpense;

        var summary = new Table().Border(TableBorder.Rounded).Title("[bold cyan]📊 Общая сводка[/]");
        summary.AddColumn("Доходы");
        summary.AddColumn("Расходы");
        summary.AddColumn("Баланс");
        summary.AddRow(
            $"[green]{totalIncome:0.00}[/]",
            $"[red]{totalExpense:0.00}[/]",
            $"[yellow]{net:0.00}[/]"
        );
        AnsiConsole.Write(summary);

        // --- Расходы по категориям ---
        var expenses = ops
            .Where(o => o.Amount < 0)
            .GroupBy(o => o.Category?.Name ?? "Без категории")
            .Select(g => new { Category = g.Key, Sum = Math.Abs(g.Sum(x => x.Amount)) })
            .OrderByDescending(x => x.Sum)
            .ToList();

        if (expenses.Any())
        {
            AnsiConsole.MarkupLine("\n[bold yellow]📉 Расходы по категориям[/]");
            var total = expenses.Sum(x => x.Sum);

            var bar = new BarChart()
                .Width(70)
                .Label("[red]Доля расходов по категориям[/]")
                .CenterLabel();

            foreach (var e in expenses)
            {
                var color = PickColor(e.Category);
                var percent = (float)(e.Sum / total * 100);
                bar.AddItem($"{e.Category} ({percent:0.0}%)", (float)e.Sum, color);
            }

            AnsiConsole.Write(bar);
        }

        // --- Доходы по категориям ---
        var incomes = ops
            .Where(o => o.Amount > 0)
            .GroupBy(o => o.Category?.Name ?? "Без категории")
            .Select(g => new { Category = g.Key, Sum = g.Sum(x => x.Amount) })
            .OrderByDescending(x => x.Sum)
            .ToList();

        if (incomes.Any())
        {
            AnsiConsole.MarkupLine("\n[bold green]💰 Доходы по категориям[/]");
            var total = incomes.Sum(x => x.Sum);

            var bar = new BarChart()
                .Width(70)
                .Label("[green]Доля доходов по категориям[/]")
                .CenterLabel();

            foreach (var i in incomes)
            {
                var color = PickColor(i.Category);
                var percent = (float)(i.Sum / total * 100);
                bar.AddItem($"{i.Category} ({percent:0.0}%)", (float)i.Sum, color);
            }

            AnsiConsole.Write(bar);
        }

        // --- Динамика по датам ---
        var byDate = ops.GroupBy(o => o.OccurredOn)
                        .Select(g => new { Date = g.Key, Sum = g.Sum(x => x.Amount) })
                        .OrderBy(g => g.Date)
                        .ToList();

        if (byDate.Any())
        {
            AnsiConsole.MarkupLine("\n[bold cyan]📅 Динамика по датам[/]");
            var bar = new BarChart()
                .Width(70)
                .Label("[yellow]Изменение баланса по датам[/]")
                .CenterLabel();

            foreach (var d in byDate)
            {
                var color = d.Sum >= 0 ? Color.Green : Color.Red;
                bar.AddItem(d.Date.ToString("dd.MM"), (float)Math.Abs(d.Sum), color);
            }

            AnsiConsole.Write(bar);
        }

        AnsiConsole.WriteLine();
        AnsiConsole.MarkupLine("[gray]Нажмите любую клавишу для возврата...[/]");
        Console.ReadKey(true);
    }
    
    private async Task ImportExportMenuAsync()
    {
        if (!EnsureUser()) return;

        var action = AnsiConsole.Prompt(
            new SelectionPrompt<string>()
                .Title("[bold cyan]📂 Импорт / Экспорт[/]")
                .AddChoices("⬆️ Экспорт данных", "⬇️ Импорт данных", "⬅️ Назад")
        );

        if (action.StartsWith("⬅️")) return;

        if (action.StartsWith("⬆️"))
        {
            // 1️⃣ Выбор формата
            var format = AnsiConsole.Prompt(
                new SelectionPrompt<string>()
                    .Title("Выберите формат экспорта:")
                    .AddChoices("📁 CSV (папка)", "🧾 JSON (файл)", "📜 YAML (файл)")
            );

            string path;

            if (format.StartsWith("📁"))
            {
                path = AnsiConsole.Ask<string>("Введите путь к папке для CSV:", "./export_csv");
                Directory.CreateDirectory(path);
            }
            else
            {
                var defaultFile = format.StartsWith("🧾") ? "./export.json" : "./export.yaml";
                path = AnsiConsole.Ask<string>($"Введите путь к файлу ({defaultFile}):", defaultFile);
                var dir = Path.GetDirectoryName(path);
                if (!string.IsNullOrWhiteSpace(dir))
                    Directory.CreateDirectory(dir);
            }

            // 2️⃣ Получаем данные для экспорта
            var accounts = await _accounts.GetByUserAsync(_currentUser!.Id);
            var categories = await _categories.GetByUserAsync(_currentUser.Id);
            var operations = await _operations.GetByUserAsync(_currentUser.Id);

            // 3️⃣ Выполняем экспорт (MultiFormatExportVisitor сам определяет формат)
            await _export.VisitAsync(_currentUser, accounts, categories, operations, path);

            AnsiConsole.MarkupLine($"[green]✅ Данные экспортированы в {Markup.Escape(path)}[/]");
        }
        else if (action.StartsWith("⬇️"))
        {
            var path = AnsiConsole.Ask<string>(
                "Введите путь, откуда импортировать (папка или файл .csv / .json / .yaml):",
                "./import");

            try
            {
                await _import.ImportAsync(path);
                AnsiConsole.MarkupLine("[green]✅ Импорт завершен успешно[/]");
            }
            catch (Exception ex)
            {
                AnsiConsole.MarkupLine($"[red]❌ Ошибка импорта: {Markup.Escape(ex.Message)}[/]");
            }
        }
    }

    private bool EnsureUser()
    {
        if (_currentUser != null) return true;
        AnsiConsole.MarkupLine("[yellow]⚠️ Сначала выберите пользователя (меню 👤)[/]");
        return false;
    }
    
    private static Color PickColor(string key)
    
    {
        // Простая палитра для разнообразия категорий
        var palette = new[]
        {
            Color.Red1, Color.Green1, Color.Blue1, Color.Yellow1,
            Color.Magenta1, Color.Cyan1, Color.Orange1, Color.LightSlateGrey,
            Color.MediumVioletRed, Color.DeepSkyBlue1
        };

        var index = Math.Abs(key.GetHashCode()) % palette.Length;
        return palette[index];
    }
}