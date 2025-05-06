using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
//using Newtonsoft.Json;
using System.Text.Json;



namespace suap_api_server.Models.Datagrams;

public class DatagramBody{
	public DatagramBody(){

    }

	public int request_type{ //tip poruke
        get;
        set;
    }

	public int device_type{ //tip periferije
        get;
        set;
    }
    public int logical_clock{ //tip mreže
        get;
        set;
    }
    public int device_id {
        get;
        set;
    }
    
   // [AllowNull]

    public BodyData? data{
        get;
        set;
    }

}
