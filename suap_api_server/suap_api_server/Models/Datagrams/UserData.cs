using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
//using Newtonsoft.Json;
using System.Text.Json;

//using System.Web.Razor.Parser.SyntaxTree;


namespace suap_api_server.Models.Datagrams;

public class UserData: BodyData{
	public UserData(){
        
    }
   // public override DatagramDataType type => DatagramDataType.User;


    public string message{
        get;
        set;
    }

    public bool input_required{
        get;
        set;
    }
        public int user_input{
        get;
        set;
    }

}
