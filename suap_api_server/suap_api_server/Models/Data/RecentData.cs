using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.AspNetCore.Authorization;
using Newtonsoft.Json;

namespace suap_api_server.Models;
public class RecentData{

    public RecentData(int a, int b, double c){
        zadnja_vrijednost = a;
        brojac = b;
        prosjek = c;
    }

    public int zadnja_vrijednost{
        get;
        set;
    }

    public int brojac{
        get;
        set;
    }

    public double prosjek{
        get;
        set;
    }
}