package hu.bme.aut.android.onlabor_2023_android.network

import hu.bme.aut.android.onlabor_2023_android.model.ApiData
import hu.bme.aut.android.onlabor_2023_android.model.ApiFrame
import retrofit2.Call
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

object NetworkManager {
    private val retrofit: Retrofit
    private val endrichData: EndrichApiInterface

    private const val URL = "http://cloud.e-iot.info:82"

    init {
        retrofit = Retrofit.Builder()
            .baseUrl(URL)
            .addConverterFactory(GsonConverterFactory.create())
            .build()
        endrichData = retrofit.create(EndrichApiInterface::class.java)
    }

    fun getApiFrame(): Call<ApiFrame?>?{
        return endrichData.getData("867420040879142")
    }
}