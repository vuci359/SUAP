/*

//  TODO 4.1  Registracija
    @RequestMapping(value = "/register", method = RequestMethod.POST)
    public ResponseEntity<?> register(@RequestBody Sensor sensor) {
        if (sensorService.register(sensor)) {
            return ResponseEntity.status(HttpStatus.CREATED).header(HttpHeaders.LOCATION, sensor.getLocation()).build();
        }
        else {
            return null;
        }
    }
    //  TODO 4.4  Popis senzora
    @RequestMapping(value = "/getSensors", method = RequestMethod.GET)
    public List<Sensor> getSensors() {
        return sensorService.getSensors();

    }
*/
using Microsoft.AspNetCore.Mvc;
using System.Collections.Generic;
using Microsoft.AspNetCore.Cors;
using Microsoft.EntityFrameworkCore;
using Swashbuckle.AspNetCore.Annotations;
using static Microsoft.AspNetCore.Http.StatusCodes;

using suap_api_server.Models;
using suap_api_server.Models.Data;
namespace suap_api_server.Controllers;

[ApiController]
[Route("api/[controller]")]
[Produces("application/json")]

public class  RegistrationController: ControllerBase
{
    private static Array<EndDevice> = new Array<EndDevice>(); //static jer se kontroleri stvaraju odvojeno za svaki poziv

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
            System.Console.WriteLine("Poslal sam: "+_data.prosjek);
            return Ok(_data);
        }catch(Exception e){
            Console.WriteLine(e.Message);
            return NotFound();
        }
    }

    [HttpGet]
    [ActionName("GetPeers")]
    [SwaggerResponse(Status200OK)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResultGetPeers([FromBody] int broj){

        try{
            _data.brojac = _data.brojac+1;
            _data.zadnja_vrijednost = broj;
            _data.prosjek = (_data.prosjek+broj)/2;
            System.Console.WriteLine("Dobil sam: "+_data.prosjek);

            return Ok(broj);
        }catch(Exception e){
            System.Console.WriteLine(e);
            
            return NotFound();
        }
    }
}