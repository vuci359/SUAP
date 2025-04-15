using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.AspNetCore.Authorization;
using Newtonsoft.Json;


namespace suap_api_server.Models.Devices;

public class Device{
	public Device(){}

	public int device_type{
        get;
        set;
    }

	public int device_id{
        get;
        set;
    }

}
