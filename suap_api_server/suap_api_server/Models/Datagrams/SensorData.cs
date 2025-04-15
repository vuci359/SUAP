using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
using Newtonsoft.Json;


namespace suap_api_server.Models.Datagrams;

public class SensorData: BodyData{
	public SensorData(){
        
    }

    public int measurement{
        get;
        set;
    }

    public string unit{
        get;
        set;
    }

}
