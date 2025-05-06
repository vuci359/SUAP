using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
using System.Text.Json.Serialization;
using System.Text.Json;

//using System.Web.Razor.Parser.SyntaxTree;


namespace suap_api_server.Models.Datagrams;

public class ActuatorData: BodyData{
	public ActuatorData(){
        
    }
 //   public override DatagramDataType type => DatagramDataType.Actuator;

    public int old_state{
        get;
        set;
    }

    public int new_state{
        get;
        set;
    }

}
