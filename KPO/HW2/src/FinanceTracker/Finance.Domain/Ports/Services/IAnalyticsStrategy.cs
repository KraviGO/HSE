namespace Finance.Domain.Ports.Services;

public interface IAnalyticsStrategy<TIn, TOut>
{
    TOut Analyze(TIn input);
}