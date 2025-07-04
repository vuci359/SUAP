using Microsoft.AspNetCore.Mvc;
using System.Collections.Generic;
using Microsoft.AspNetCore.Cors;
using Microsoft.EntityFrameworkCore;
using Swashbuckle.AspNetCore.Annotations;
using static Microsoft.AspNetCore.Http.StatusCodes;

using suap_api_server.Models;
using suap_api_server.Models.Data;
using suap_api_server.Models.Devices;
using suap_api_server.Models.Datagrams;
//using suap_api_server.Converters;

namespace suap_api_server.Controllers;

[ApiController]
[Route("api/[controller]")]
[Produces("application/json")]

public class  DatagramController: ControllerBase
{
    private static Dictionary<int,Queue<MainDatagram>> _message_queue = new Dictionary<int,Queue<MainDatagram>>(); //static jer se kontroleri stvaraju odvojeno za svaki poziv
    

    public DatagramController()
    {
     //  _data = new RecentData(0,0,0);
    }

    [HttpPost]
    [ActionName("PushMessage")]
    [Route("PushMessage")]
    [SwaggerResponse(Status208AlreadyReported)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResult PushMessage([FromBody] MainDatagram json_data){
        try{
         if(!_message_queue.ContainsKey(json_data.targetID)){
            Queue<MainDatagram> queue = new Queue<MainDatagram>();
            _message_queue.Add(json_data.targetID, queue);
         }
            _message_queue[json_data.targetID].Enqueue(json_data);
            return Ok(json_data.ID);
        }catch(Exception e){
            Console.WriteLine(e.Message);
            return NotFound();
        }
    }

    [HttpGet("PopMessage/{deviceID}")]
    //[ActionName("PopMessage")]
    //[Route("PopMessage")]
    [SwaggerResponse(Status200OK)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResult PopMessage(int deviceID){

       try{
         if(!_message_queue.ContainsKey(deviceID)){
            return NotFound();
         }else{
            return Ok(_message_queue[deviceID].Dequeue());
         }
        }catch(Exception e){
            Console.WriteLine(e.Message);
            return NotFound();
        }
    }
    [HttpGet("MessageCount/{deviceID}")]
    //[ActionName("MessageCount")]
    //[Route("MessageCount")]
    [SwaggerResponse(Status200OK)]
    [SwaggerResponse(Status404NotFound)]
    public IActionResult MessageCount(int deviceID){

       try{
         if(!_message_queue.ContainsKey(deviceID)){
            return Ok(0); //nema poruka za čvor
         }else{
            return Ok(_message_queue[deviceID].Count);
         }
        }catch(Exception e){
            Console.WriteLine(e.Message);
            return NotFound();
        }
    }
}
