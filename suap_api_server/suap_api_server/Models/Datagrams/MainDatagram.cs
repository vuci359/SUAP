using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Diagnostics.CodeAnalysis;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
using Newtonsoft.Json;


namespace suap_api_server.Models.Datagrams;

public class MainDatagram{
	public MainDatagram(){

    }

	public string ID{ //ID poruke
        get;
        set;
    }

	public string network{ //ID mreže
        get;
        set;
    }
    public int network_type{ //tip mreže
        get;
        set;
    }
    [JsonPropertyName("interface")]
    public string _interface {
        get;
        set;
    }
    public int sourceID{
        get;
        set;
    } 
    public int targetID{
        get;
        set;
    }
    public DatagramBody body{
        get;
        set;
    }

}
