using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
using Newtonsoft.Json;


namespace suap_api_server.Models.Datagrams;

public class ActuatorData: BodyData{
	public ActuatorData(){
        
    }

    public int old_state{
        get;
        set;
    }

    public int new_state{
        get;
        set;
    }

}
