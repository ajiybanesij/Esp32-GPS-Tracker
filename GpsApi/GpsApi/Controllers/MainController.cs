using GpsApi.Content;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web.Http;

namespace GpsApi.Controllers
{
    [RoutePrefix("api")]
    public class MainController : ApiController
    {
        [Route("add/{latData}/{longData}/{dateData}")]
        [HttpGet,HttpPost]
        public  string AddGPS(string latData,string longData,string dateData)
        {
            using (DB db=new DB())
            {
                try
                {
                    KonumTablo model = new KonumTablo()
                    {
                        latData = latData,
                        longData = longData,
                        daeData = dateData
                    };

                    db.KonumTabloes.Add(model);
                    db.SaveChanges();
                    return "0";

                }catch(Exception e)
                {
                    return "-1";
                }
            }
        }
    }
}
