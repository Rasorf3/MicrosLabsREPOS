package com.example.trashbinmicros

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.example.trashbinmicros.databinding.ActivityMainBinding
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var btService: BluetoothService
    private val macHC06 = "00:11:22:33:44:55" // cambia por tu MAC real
    private val dataSets = mutableListOf<LineDataSet>()
    private var timeIndex = 0f

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // 1) Infla y asocia el layout
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // 2) Solicita permisos de ubicación si faltan
        val perms = arrayOf(
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.ACCESS_COARSE_LOCATION
        )
        if (perms.any { checkSelfPermission(it) != PackageManager.PERMISSION_GRANTED }) {
            ActivityCompat.requestPermissions(this, perms, 1)
        }

        // 3) Inicializa los 5 LineCharts
        arrayOf(binding.chart0, binding.chart1, binding.chart2,
            binding.chart3, binding.chart4).forEachIndexed { i, chart ->
            val set = LineDataSet(mutableListOf(), "Var#$i").apply {
                lineWidth = 2f; setDrawCircles(false)
            }
            dataSets += set
            chart.data = LineData(set)
            chart.description.isEnabled = false
            chart.axisRight.isEnabled = false
            chart.xAxis.granularity = 1f              // distancia mínima entre dos labels en el eje X
            chart.xAxis.isGranularityEnabled = true   // habilita la granularidad

        }

        // 4) Configura BluetoothService
        btService = BluetoothService(
            onConnected    = { runOnUiThread { binding.btnConnect.text = "OK" } },
            onDisconnected = { runOnUiThread { binding.btnConnect.text = "Conectar" } },
            onDataReceived = { line -> runOnUiThread { handleLine(line) } }
        )

        // 5) Botones de conexión y envío
        binding.btnConnect.setOnClickListener    { btService.connect(macHC06) }
        binding.btnDisconnect.setOnClickListener { btService.disconnect()     }
        binding.btnSend.setOnClickListener       {
            val msg = binding.editSend.text.toString()
            btService.send(msg + "\n")
        }
    }

    private fun handleLine(line: String) {
        val parts = line.split("_")
        parts.forEachIndexed { i, p ->
            if (i < dataSets.size) {
                p.toFloatOrNull()?.let { v ->
                    dataSets[i].addEntry(Entry(timeIndex, v))
                    val chart = listOf(
                        binding.chart0, binding.chart1,
                        binding.chart2, binding.chart3,
                        binding.chart4
                    )[i]
                    chart.data.notifyDataChanged()
                    chart.notifyDataSetChanged()
                    chart.moveViewToX(timeIndex)
                }
            }
        }
        timeIndex += 1f
    }
}

