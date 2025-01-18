using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Cors;
using Microsoft.EntityFrameworkCore;
using Swashbuckle.AspNetCore.Annotations;
using static Microsoft.AspNetCore.Http.StatusCodes;

using suap_api_server.Models;
namespace suap_api_server.Controllers;

[ApiController]
[Route("api/[controller]")]
[Produces("application/json")]

public class  SuapApiController: ControllerBase
{
    private RecentData _data = new RecentData(0,5,5);

    public SuapApiController()
    {
     //  _data = new RecentData(0,0,0);
    }
    [HttpGet]
    [ActionName("GetData")]
    [SwaggerResponse(Status200OK)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResult GetData(){
        try{
            return Ok(_data);
        }catch(Exception e){
            Console.WriteLine(e.Message);
            return NotFound();
        }
    }

    [HttpPost]
    [ActionName("PostData")]
    [SwaggerResponse(Status200OK)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResult PostData([FromBody] int broj){

        try{
            _data.brojac = _data.brojac+1;
            _data.zadnja_vrijednost = broj;
            _data.prosjek = (_data.prosjek+broj)/2;
            System.Console.WriteLine("Dobil sam: "+broj);

            return Ok();
        }catch(Exception e){
            System.Console.WriteLine(e);
            
            return NotFound();
        }
    }
}