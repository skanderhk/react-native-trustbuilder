package com.trustbuilder

import android.util.Log
import com.inwebo.iwlib.WebServiceCall
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL

class SimpleWebServiceCall : WebServiceCall {
    companion object {
        private const val TAG = "SimpleWebServiceCall"
    }

    override fun WebServiceCall(url: String?, timeout: Long): String? {
        Log.d(TAG, "WebServiceCall: $url, timeout=$timeout")
        return try {
            val connection = URL(url).openConnection() as HttpURLConnection
            connection.requestMethod = "GET"
            connection.connectTimeout = timeout.toInt()
            connection.readTimeout = timeout.toInt()
            
            val responseCode = connection.responseCode
            Log.d(TAG, "Response code: $responseCode")
            
            if (responseCode == HttpURLConnection.HTTP_OK) {
                val reader = BufferedReader(InputStreamReader(connection.inputStream))
                val response = reader.use { it.readText() }
                Log.d(TAG, "Response length: ${response.length}")
                response
            } else {
                Log.e(TAG, "HTTP error: $responseCode")
                ""
            }
        } catch (e: Exception) {
            Log.e(TAG, "Network error: ${e.message}", e)
            ""
        }
    }
}
