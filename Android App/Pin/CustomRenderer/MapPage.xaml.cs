using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Http;
using Xamarin.Forms;
using Xamarin.Forms.Maps;

namespace CustomRenderer
{
    public partial class MapPage : ContentPage
    {
       
        public MapPage()
        {
            InitializeComponent();
            GetData();
        }
         public async void GetData()
         {
             KonumTablo model = new KonumTablo();
             string URL = "";

             var request = new HttpRequestMessage(HttpMethod.Get, URL + "/getlast");

             using (HttpClient client = new HttpClient())
             {
                 var response = await client.SendAsync(request);

                 if (response.StatusCode == HttpStatusCode.BadRequest)
                 {
                     model = null;
                 }

                 else if (response.StatusCode == HttpStatusCode.OK)
                 {
                     var content = await response.Content.ReadAsStringAsync();
                     model = JsonConvert.DeserializeObject<KonumTablo>(content);
                 }
                 else
                 {
                     model = null;
                 }

             }
             double latData = (Convert.ToDouble(model.LatData))/1000000;
             double longData = (Convert.ToDouble(model.LongData))/1000000;
            DateTime newDate = new DateTime(model.DateTime.Year, model.DateTime.Month,
                    model.DateTime.Day, model.DateTime.Hour + 3, model.DateTime.Minute, model.DateTime.Second);
             var pin = new CustomPin
             {
                 Type = PinType.Place,
                 Position = new Position(latData,longData),
                 Label = "Son Konum",
                 Address = newDate.ToString(),
                 Id = "Xamarin",
                 Url = "http://xamarin.com/about/"
             };

            customMap.Pins.Add(pin);
            customMap.MoveToRegion(MapSpan.FromCenterAndRadius(new Position(latData, longData), Distance.FromMiles(1.0)));

         }
    }
}
