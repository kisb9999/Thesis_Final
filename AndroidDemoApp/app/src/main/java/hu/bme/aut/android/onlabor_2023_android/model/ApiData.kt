package hu.bme.aut.android.onlabor_2023_android.model

//This data class stores the ApiFrame data JSON
data class ApiData (
    var skip: Boolean,
    var A: String,
    var T: String,
    var label: String,
    var gpsdata: String
)