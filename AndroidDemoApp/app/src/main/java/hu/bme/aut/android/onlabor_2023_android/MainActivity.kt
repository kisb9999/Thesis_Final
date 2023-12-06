package hu.bme.aut.android.onlabor_2023_android

import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import com.github.mikephil.charting.charts.LineChart
import com.github.mikephil.charting.components.Description
import com.github.mikephil.charting.components.LimitLine
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet
import com.github.mikephil.charting.formatter.IndexAxisValueFormatter
import hu.bme.aut.android.onlabor_2023_android.databinding.ActivityMainBinding
import hu.bme.aut.android.onlabor_2023_android.model.ApiFrame
import hu.bme.aut.android.onlabor_2023_android.network.NetworkManager
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private val tempUpperLimit = 24f
    private val tempLowerLimit = 21F

    private val lightUpperLimit = 90F
    private val lightLowerLimit = 30F

    override fun onCreate(savedInstanceState: Bundle?) {
        try {
            Thread.sleep(2000)
        } catch (e: InterruptedException) {
            e.printStackTrace()
        }
        setTheme(R.style.Theme_Onlabor_2023_Android)
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        loadDataFromApi()
    }

    private fun loadDataFromApi(){
        //The NetworkManager tries to get the JSON. If the call is successful the onResponse method is called otherwise the onFailure will be called.
        NetworkManager.getApiFrame()?.enqueue(object : Callback<ApiFrame?> {
            override fun onResponse(call: Call<ApiFrame?>, response: Response<ApiFrame?>) {
                Log.d("MainActivity", "onResponse: " + response.code())
                if (response.isSuccessful) {
                    //If everything went perfectly we call upon the displayData method that will fill up the charts with the json data.
                    displayData(response.body())
                }
                else{
                    Log.d("MainActivity", "onResponse else was called!")
                    Toast.makeText(this@MainActivity, "Error: " + response.message(), Toast.LENGTH_LONG).show()
                }

            }
            override fun onFailure(call: Call<ApiFrame?>, t: Throwable) {
                Log.d("MainActivity", "onFailure was called!")
                t.printStackTrace()
                Toast.makeText(this@MainActivity, "An error occurred!", Toast.LENGTH_LONG).show()
            }
        })
    }

    private fun displayData(receivedData: ApiFrame?){
        //Set up some chart properties
        chartSettings(binding.tempChart)
        chartSettings(binding.lightChart)

        //Load data from api into ArrayLists --> these will be the data points in the chart
        val entries_temp = ArrayList<Entry>()
        val entries_light = ArrayList<Entry>()
        val labels = ArrayList<String>()

        for(i in 0 until receivedData?.data?.size!!){
            val data = receivedData.data[receivedData.data.size-1-i]
            //Here we add the temperature data to the entries
            entries_temp.add(Entry(i.toFloat(), data.T.toFloat()/100))
            //Here we add the ambient light data to the entries
            entries_light.add(Entry(i.toFloat(), data.A.toFloat()/100))
            labels.add(data.label.subSequence(11, 19).toString())
        }

        //Get the time interval in which the data was measured and add it to the description label
        val s = receivedData.data[receivedData?.data?.size!!-1].label + " - "+ receivedData.data[0].label
        binding.tempChart.description = setDescription(s)
        binding.lightChart.description = setDescription(s)

        //Create data sets from the entries that will be displayed in the charts
        val dataSetTemp = LineDataSet(entries_temp, "Temperature Data (in C°)")
        val dataSetLight = LineDataSet(entries_light, "Ambient Light Data (in lux)")
        dataSetSettings(dataSetTemp)
        dataSetSettings(dataSetLight)

        //Create line segments in the charts
        val lineDataTemp = LineData(dataSetTemp)
        val lineDataLight = LineData(dataSetLight)
        binding.tempChart.data = lineDataTemp
        binding.lightChart.data = lineDataLight

        //Add constant lines into the charts resembling the upper and lower limits of the temperature data and the ambient light data
        //Under and above these lines some action will need to take place in the real life
        //Eg: if the temperature is above the red line the user will need to start the air conditioner
        val lineUpperTemp = LimitLine(tempUpperLimit, "")
        val lineLowerTemp = LimitLine(tempLowerLimit, "")
        val lineUpperLight = LimitLine(lightUpperLimit, "")
        val lineLowerLight = LimitLine(lightLowerLimit, "")

        //Set some line properties (color, line width)
        limitSettings(lineUpperTemp, Color.RED)
        limitSettings(lineLowerTemp, Color.BLUE)
        limitSettings(lineUpperLight, Color.YELLOW)
        limitSettings(lineLowerLight, Color.DKGRAY)

        //Adding the lines into the charts
        val leftaxis1 = binding.tempChart.axisLeft
        leftaxis1.addLimitLine(lineUpperTemp)
        leftaxis1.addLimitLine(lineLowerTemp)

        val leftaxis2 = binding.lightChart.axisLeft
        leftaxis2.addLimitLine(lineUpperLight)
        leftaxis2.addLimitLine(lineLowerLight)


        //Adds the timestamps to the X axis of both charts
        val xAxisTemp = binding.tempChart.xAxis
        val xAxisLight = binding.lightChart.xAxis
        //val list = labels.toMutableList().apply { reverse() }
        xAxisTemp.valueFormatter = IndexAxisValueFormatter(labels)
        xAxisLight.valueFormatter = IndexAxisValueFormatter(labels)


        binding.tempChart.invalidate()
        binding.lightChart.invalidate()
    }


    private fun chartSettings(chart: LineChart){
        chart.setBackgroundColor(Color.LTGRAY)
        chart.setBorderColor(Color.BLACK)
        chart.setTouchEnabled(true)
        chart.enableScroll()
    }

    private fun setDescription(string: String): Description{
        val description = Description()
        description.text = string
        description.textSize = 12.5F
        return description
    }

    private fun dataSetSettings(dataSet: LineDataSet){
        dataSet.color = Color.BLACK
        //dataSet.setCircleColor(color)
        dataSet.setDrawCircleHole(false)
        dataSet.setDrawCircles(false)
    }

    private fun limitSettings(line: LimitLine, color: Int){
        line.lineColor = color
        line.lineWidth = 1.5f
    }
}