using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.AspNetCore.Authorization;
using Newtonsoft.Json;

namespace suap_api_server.Models.Devices;

public class EndDevice{
	public EndDevice(){}

	public int ID{
        get;
        set;
    }

	public string IP{
        get;
        set;
    }

	public string MAC{
        get;
        set;
    }

	public List<Device> devices{
		get;
		set;
	}

}
