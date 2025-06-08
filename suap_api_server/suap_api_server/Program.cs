//using Newtonsoft.Json;
//using suap_api_server.Converters;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
//builder.Services.AddMvcCore().AddNewtonsoftJson();
builder.Services.AddControllersWithViews().AddJsonOptions(options => {
        options.AllowInputFormatterExceptionMessages = true;
        //options.JsonSerializerOptions.Converters.Add(new PolymorphicDatagramConverter());
    });

builder.Services.AddSwaggerGen();


//config.Formatters.JsonFormatter.SerializerSettings.Converters.Add(new PolymorphicDatagramConverter());


var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Home/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

//app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

app.UseAuthorization();

app.UseSwagger();
app.UseSwaggerUI();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Index}/{id?}");

app.Run();
