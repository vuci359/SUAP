using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text.Json.Serialization;
using System.Text.Json;
//using System.Web.Razor.Parser.SyntaxTree;

using Microsoft.AspNetCore.Authorization;
using Microsoft.Identity.Client;
//using Newtonsoft.Json;


namespace suap_api_server.Models.Datagrams;

[JsonPolymorphic(TypeDiscriminatorPropertyName = "type")]
[JsonDerivedType(typeof(SensorData), 0)]
[JsonDerivedType(typeof(UserData), 2)]
[JsonDerivedType(typeof(ActuatorData), 1)]
//[JsonDerivedType(typeof(SensorData))]
//[JsonDerivedType(typeof(UserData))]
//[JsonDerivedType(typeof(ActuatorData))]
public abstract class BodyData{

 //  public enum DatagramDataType {
 //       Sensor,
 //       Actuator,
 //       User
 //   };
	
 //   [JsonConverter(typeof(JsonStringEnumConverter))]
  //  public abstract DatagramDataType type { get; }

}
