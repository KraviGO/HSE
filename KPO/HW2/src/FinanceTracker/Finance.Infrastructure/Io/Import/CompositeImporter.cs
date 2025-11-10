using Finance.Domain.Ports.Services;

namespace Finance.Infrastructure.Io.Import;

public sealed class CompositeImporter : IImportService
{
    private readonly IImportService _csv;
    private readonly IImportService _json;
    private readonly IImportService _yaml;

    public CompositeImporter(CsvImporter csv, JsonImporter json, YamlImporter yaml)
    {
        _csv = csv;
        _json = json;
        _yaml = yaml;
    }

    public Task ImportAsync(string path, CancellationToken ct = default)
    {
        if (string.IsNullOrWhiteSpace(path))
            throw new ArgumentException("Import path is required.");

        var ext = Path.GetExtension(path).ToLowerInvariant();
        return ext switch
        {
            ".csv" => _csv.ImportAsync(path, ct),
            ".json" => _json.ImportAsync(path, ct),
            ".yml" or ".yaml" => _yaml.ImportAsync(path, ct),
            _ => DetectAndImport(path, ct)
        };
    }
    
    private Task DetectAndImport(string path, CancellationToken ct)
    {
        if (Directory.Exists(path))
            return _csv.ImportAsync(path, ct);
        
        return _json.ImportAsync(path, ct);
    }
}