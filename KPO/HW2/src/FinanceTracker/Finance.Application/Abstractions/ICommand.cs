namespace Finance.Application.Abstractions;

public interface ICommand<T>
{
    Task<T> Execute(CancellationToken ct = default);
}