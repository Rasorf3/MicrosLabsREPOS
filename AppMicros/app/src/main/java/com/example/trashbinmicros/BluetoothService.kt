package com.example.trashbinmicros

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.OutputStream
import java.util.*
import kotlin.concurrent.thread

class BluetoothService(
    private val onConnected: ()->Unit,
    private val onDisconnected: ()->Unit,
    private val onDataReceived:(String)->Unit
) {
    private val adapter = BluetoothAdapter.getDefaultAdapter()
    private var socket: BluetoothSocket? = null
    private var outStream: OutputStream? = null
    private val UUID_SPP = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    fun connect(mac: String) {
        thread {
            try {
                val device = adapter.getRemoteDevice(mac)
                socket = device.createRfcommSocketToServiceRecord(UUID_SPP)
                adapter.cancelDiscovery()
                socket!!.connect()
                outStream = socket!!.outputStream
                onConnected()
                listenInput()
            } catch (_: Exception) {
                disconnect()
            }
        }
    }

    fun disconnect() {
        thread {
            socket?.close()
            socket = null
            onDisconnected()
        }
    }

    fun send(data: String) {
        thread {
            outStream?.write(data.toByteArray())
        }
    }

    private fun listenInput() {
        val reader = BufferedReader(InputStreamReader(socket!!.inputStream))
        try {
            while (socket?.isConnected == true) {
                val line = reader.readLine() ?: break
                onDataReceived(line)
            }
        } catch (_: Exception) { }
        disconnect()
    }
}
