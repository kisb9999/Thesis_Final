package hu.bme.aut.android.onlabor_2023_android.model

//This data class stores the API response
data class ApiFrame (
    val msg: String? = null,
    val lastId: String? = null,
    val data: List<ApiData>? = null, //the list stores ApiData type JSONs
    val labels: List<String>? = null
)