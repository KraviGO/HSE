using System.Diagnostics;
using Finance.Application.Abstractions;

namespace Finance.Application.Decorators;

public sealed class TimingCommandDecorator<T> : ICommand<T> {
    private readonly ICommand<T> _inner; private readonly Action<TimeSpan,string> _onMeasured; private readonly string _name;
    public TimingCommandDecorator(ICommand<T> inner, Action<TimeSpan,string> onMeasured, string name) {
        _inner = inner; _onMeasured = onMeasured; _name = name;
    }
    public async Task<T> Execute(CancellationToken ct = default) {
        var sw = Stopwatch.StartNew(); var result = await _inner.Execute(ct); sw.Stop();
        _onMeasured(sw.Elapsed, _name); return result;
    }
}