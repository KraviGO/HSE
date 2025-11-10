using System.Diagnostics;
using Finance.Application.Abstractions;

namespace Finance.Application.Commands.Common;

public sealed class TimingCommandDecorator<TResponse> : ICommand<TResponse>
{
    private readonly ICommand<TResponse> _inner;
    private readonly Action<TimeSpan, string> _onMeasured;
    private readonly string _name;

    public TimingCommandDecorator(
        ICommand<TResponse> inner,
        Action<TimeSpan, string> onMeasured,
        string name)
    {
        _inner = inner;
        _onMeasured = onMeasured;
        _name = name;
    }

    public async Task<TResponse> Execute(CancellationToken ct = default)
    {
        var sw = Stopwatch.StartNew();
        var result = await _inner.Execute(ct);
        sw.Stop();

        _onMeasured?.Invoke(sw.Elapsed, _name);
        return result;
    }
}
