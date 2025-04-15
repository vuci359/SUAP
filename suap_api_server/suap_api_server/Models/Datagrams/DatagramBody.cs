using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
using Newtonsoft.Json;


namespace suap_api_server.Models.Datagrams;

public class DatagramBody{
	public DatagramBody(){

    }

	public int request_type{ //ID poruke
        get;
        set;
    }

	public int device_type{ //ID mreže
        get;
        set;
    }
    public int logical_clock{ //tip mreže
        get;
        set;
    }
    public string device_id {
        get;
        set;
    }
    public BodyData data{
        get;
        set;
    }

}
