using Microsoft.AspNetCore.Mvc;
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

    public RegistrationController()
    {
     //  _data = new RecentData(0,0,0);
    }
    [HttpPost]
    [ActionName("Register")]
    [SwaggerResponse(Status200OK)]
    [SwaggerResponse(Status208AlreadyReported)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResult Register(){
        try{
            var node = new EndDevice();
            if(_nodes.Contains(node)){
                return StatusCode(208);
            }else{
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