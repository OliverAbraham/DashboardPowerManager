# Dashboard power manager

## OVERVIEW

Queries the openweathermap.org Web API to get the current weather and forecast.


## License

Licensed under Apache licence.
https://www.apache.org/licenses/LICENSE-2.0


## Compatibility

The nuget package was build with DotNET 8.



## INSTALLATION

Install the Nuget package "Abraham.OpenWeatherMapClient" into your application (from https://www.nuget.org).

Add the following code:
```C#
    var client = new OpenWeatherMapConnector()
        .UseApiKey(_myApiKey)
        .UseLocation(lattitude:"53.8667", longitude:"9.8833");

    var weatherInfo = client.ReadCurrentTemperatureAndForecast();

    Console.WriteLine($"Current temperature: {weatherInfo.CurrentTemperature} {weatherInfo.Unit}");
```


That's it!

For more options, please refer to my Demo application in the github repository (see below).
The Demo and the nuget source code is well documented.



## HOW TO INSTALL A NUGET PACKAGE
This is very simple:
- Start Visual Studio (with NuGet installed) 
- Right-click on your project's References and choose "Manage NuGet Packages..."
- Choose Online category from the left
- Enter the name of the nuget package to the top right search and hit enter
- Choose your package from search results and hit install
- Done!


or from NuGet Command-Line:

    Install-Package Abraham.OpenWeatherMapClient





## AUTHOR

Oliver Abraham, mail@oliver-abraham.de, https://www.oliver-abraham.de

Please feel free to comment and suggest improvements!



## SOURCE CODE

The source code is hosted at:

https://github.com/OliverAbraham/Abraham.OpenWeatherMapClient

The Nuget Package is hosted at: 

https://www.nuget.org/packages/Abraham.OpenWeatherMapClient
