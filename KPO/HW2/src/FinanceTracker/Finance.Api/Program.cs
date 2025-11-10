using Finance.Infrastructure.Repositories;
using Finance.Application.Services;
using Finance.Api.Extensions;
using System.Text.Json.Serialization;
using Finance.Api.Extensions.EndpointExtensions;
using Finance.Domain.Ports.Repositories;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddAppDatabase("Data Source=finance.db");
builder.Services.AddAppRepositories();
builder.Services.AddScoped<IGoalRepository, EfGoalRepository>();
builder.Services.AddAppImportExport();
builder.Services.AddScoped<TransferService>();

builder.Services.AddControllers()
    .AddJsonOptions(options =>
    {
        options.JsonSerializerOptions.ReferenceHandler = ReferenceHandler.IgnoreCycles;
        options.JsonSerializerOptions.WriteIndented = true;
    });

builder.Services.AddCors(options =>
{
    options.AddPolicy("AllowReact", policy =>
    {
        policy
            .WithOrigins("http://localhost:5173")
            .AllowAnyMethod()
            .AllowAnyHeader()
            .AllowCredentials();
    });
});

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

app.UseCors("AllowReact");

if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();

app.MapUserEndpoints();
app.MapAccountEndpoints();
app.MapCategoryEndpoints();
app.MapOperationEndpoints();
app.MapGoalEndpoints();
app.MapTransferEndpoints();
app.MapAnalyticsEndpoints();
app.MapImportExportEndpoints();
app.MapControllers();

app.Run();