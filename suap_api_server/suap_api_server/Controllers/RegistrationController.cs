using Microsoft.AspNetCore.Mvc;
using System.Linq;
using System.Collections.Generic;
using Microsoft.AspNetCore.Cors;
using Microsoft.EntityFrameworkCore;
using Swashbuckle.AspNetCore.Annotations;
using static Microsoft.AspNetCore.Http.StatusCodes;

using suap_api_server.Models;
using suap_api_server.Models.Data;
using suap_api_server.Models.Devices;
namespace suap_api_server.Controllers;

[ApiController]
[Route("api/[controller]")]
[Produces("application/json")]

public class  RegistrationController: ControllerBase
{
    private static List<EndDevice> _nodes = new List<EndDevice>(); //static jer se kontroleri stvaraju odvojeno za svaki poziv

    private static Random rnd = new Random();

    public RegistrationController()
    {
     //  _data = new RecentData(0,0,0);
    }
    [HttpPost]
    [ActionName("Register")]
    [SwaggerResponse(Status200OK)]
    [SwaggerResponse(Status208AlreadyReported)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResult Register([FromBody] EndDevice node){
        try{
            //var node = new EndDevice();
            if(_nodes.Exists(x => x.ID == node.ID)){ //preraditi da uspoređuje MAC adrese i ažurira po potrebi
                return StatusCode(208); //ne podržava zamjenu IP adrese, moguće rješiti naknadno s provjerom MAC umjesto ID i vračanjem podataka uz kod 208
            }else{
                node.ID = rnd.Next(100); //pretpostavlja se do 100 uređaja; //nije idealno,moguće ponavljanje
                _nodes.Add(node);
                return Ok(node);
            }
        }catch(Exception e){
            Console.WriteLine(e.Message);
            return NotFound();
        }
    }

    [HttpGet]
    [ActionName("GetPeers")]
    [SwaggerResponse(Status200OK)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResult GetPeers(){

        try{
            return Ok(_nodes);
        }catch(Exception e){  
            return NotFound();
        }
    }
}
