package hu.bme.aut.android.onlabor_2023_android.network

import hu.bme.aut.android.onlabor_2023_android.model.ApiFrame
import retrofit2.Call
import retrofit2.http.GET
import retrofit2.http.Query

interface EndrichApiInterface {
    @GET("/iot-demo/getdata-ios.php")
    fun getData(
        @Query("IMEI") IMEI: String?
    ): Call<ApiFrame?>?
}