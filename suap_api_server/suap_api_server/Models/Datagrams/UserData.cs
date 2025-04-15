using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
using Newtonsoft.Json;


namespace suap_api_server.Models.Datagrams;

public class UserData: BodyData{
	public UserData(){
        
    }

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
