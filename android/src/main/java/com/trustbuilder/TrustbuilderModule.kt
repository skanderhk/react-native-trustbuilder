package com.trustbuilder

import android.content.Context
import android.provider.Settings
import android.util.Log
import androidx.security.crypto.EncryptedSharedPreferences
import androidx.security.crypto.MasterKey
import com.facebook.react.bridge.ReactApplicationContext
import com.inwebo.iwlib.IW
import org.json.JSONObject

class TrustbuilderModule(reactContext: ReactApplicationContext) :
  NativeRNTrustbuilderSpec(reactContext) {

  private val context: ReactApplicationContext = reactContext
  private var iw: IW? = null
  private var isInitialized = false

  companion object {
    const val NAME = "RNTrustbuilder"
    private const val STORAGE_KEY = "trustbuilder_storage_data"
    private const val PREFS_NAME = "trustbuilder_encrypted_prefs"
    private const val TAG = "TrustbuilderModule"
  }

  private fun getAndroidId(): String {
    return Settings.Secure.getString(
      context.contentResolver,
      Settings.Secure.ANDROID_ID
    ) ?: ""
  }

  private fun getEncryptedPrefs(): EncryptedSharedPreferences {
    val masterKey = MasterKey.Builder(context)
      .setKeyScheme(MasterKey.KeyScheme.AES256_GCM)
      .build()

    return EncryptedSharedPreferences.create(
      context,
      PREFS_NAME,
      masterKey,
      EncryptedSharedPreferences.PrefKeyEncryptionScheme.AES256_SIV,
      EncryptedSharedPreferences.PrefValueEncryptionScheme.AES256_GCM
    ) as EncryptedSharedPreferences
  }

  private fun requireIW(): IW {
    val instance = iw
    if (instance == null) {
      throw IllegalStateException("Trustbuilder not initialized. Call initialize() first.")
    }
    return instance
  }

  private fun initIW(config: JSONObject) {
    if (isInitialized) return

    val macId = config.getString("macId")
    val server = config.getString("server")
    val hostVersion = config.getString("hostVersion")
    val timeout = config.getLong("timeout")
    val lang = config.getString("lang")

    val sn = getAndroidId()
    val appData = "_"

    Log.d(TAG, "Initializing IW: SN=$sn, server=$server, macId=$macId, hostVersion=$hostVersion")

    try {
      iw = IW()

      Log.d(TAG, "Calling Init(sn=$sn, appData=$appData)")
      iw!!.Init(sn, appData)

      Log.d(TAG, "Setting WsServer: $server")
      iw!!.WsServerSet(server)

      Log.d(TAG, "Setting HostVersion: $hostVersion")
      iw!!.HostVersionSet(hostVersion)

      Log.d(TAG, "Setting WsTimeout: $timeout")
      iw!!.WsTimeoutSet(timeout)

      Log.d(TAG, "Setting Maccess: $macId")
      iw!!.MaccessSet(macId)

      Log.d(TAG, "Setting Lang: $lang")
      iw!!.LangSet(lang)

      val storedData = getEncryptedPrefs().getString(STORAGE_KEY, "") ?: ""
      Log.d(TAG, "Setting storage data (${storedData.length} chars)")
      iw!!.StorageDataSet(storedData)

      Log.d(TAG, "Init completed, log=${iw!!.log}")

      isInitialized = true
      Log.d(TAG, "IW initialized successfully")
    } catch (e: Exception) {
      Log.e(TAG, "Failed to initialize IW: ${e.message}", e)
      iw = null
      isInitialized = false
      throw e
    }
  }

  private fun saveStorage() {
    val instance = iw ?: return
    try {
      if (instance.StorageDataChanged() != 0L) {
        val data = instance.StorageDataGet()
        if (data != null) {
          getEncryptedPrefs().edit().putString(STORAGE_KEY, data).apply()
          Log.d(TAG, "Storage saved (${data.length} chars)")
        }
      }
    } catch (e: Exception) {
      Log.e(TAG, "Failed to save storage: ${e.message}", e)
    }
  }

  override fun initialize(config: String): String {
    try {
      val configJson = JSONObject(config)
      initIW(configJson)

      val instance = requireIW()
      val result = JSONObject()
      
      val activated = try {
        instance.IsActivated() == 1L
      } catch (e: NullPointerException) {
        Log.w(TAG, "IsActivated failed (data not initialized): ${e.message}")
        false
      }
      
      val blocked = try {
        instance.IsBlocked() == 1L
      } catch (e: NullPointerException) {
        Log.w(TAG, "IsBlocked failed (data not initialized): ${e.message}")
        false
      }
      
      result.put("isActivated", activated)
      result.put("isBlocked", blocked)
      return result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "Init error: ${e.message}", e)
      val result = JSONObject()
      result.put("isActivated", false)
      result.put("isBlocked", false)
      result.put("error", e.message ?: "Unknown error")
      return result.toString()
    }
  }

  override fun setStorageData(data: String): Boolean {
    getEncryptedPrefs().edit().putString(STORAGE_KEY, data).apply()
    return try {
      iw?.StorageDataSet(data) == 0L
    } catch (e: Exception) {
      Log.e(TAG, "setStorageData error: ${e.message}", e)
      false
    }
  }

  override fun getStorageData(): String {
    return getEncryptedPrefs().getString(STORAGE_KEY, "") ?: ""
  }

  override fun storageDataChanged(): Boolean {
    return try {
      iw?.StorageDataChanged() != 0L
    } catch (e: Exception) {
      false
    }
  }

  override fun isActivated(): Boolean {
    return try {
      iw?.IsActivated() == 1L
    } catch (e: Exception) {
      false
    }
  }

  override fun isBlocked(): Boolean {
    return try {
      iw?.IsBlocked() == 1L
    } catch (e: Exception) {
      false
    }
  }

  override fun mustUpgrade(): Boolean {
    return try {
      iw?.MustUpgrade() == 1L
    } catch (e: Exception) {
      false
    }
  }

  override fun activationStart(code: String): String {
    return try {
      requireIW().ActivationStart(code).toString()
    } catch (e: Exception) {
      Log.e(TAG, "activationStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun activationFinalize(code: String, pin: String, name: String): String {
    return try {
      val result = requireIW().ActivationFinalize(code, pin, name)
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "activationFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun resetStart(code: String): String {
    return try {
      requireIW().ResetStart(code).toString()
    } catch (e: Exception) {
      Log.e(TAG, "resetStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun resetFinalize(code: String, pin: String): String {
    return try {
      val result = requireIW().ResetFinalize(code, pin)
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "resetFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun synchronizeStart(): String {
    return try {
      requireIW().SynchronizeStart().toString()
    } catch (e: Exception) {
      Log.e(TAG, "synchronizeStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun synchronizeFinalize(pin: String): String {
    return try {
      val result = requireIW().SynchronizeFinalize(pin)
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "synchronizeFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun pwdUpdateStart(): String {
    return try {
      requireIW().PwdUpdateStart().toString()
    } catch (e: Exception) {
      Log.e(TAG, "pwdUpdateStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun pwdUpdateFinalize(newPin: String, currentPin: String): String {
    return try {
      val result = requireIW().PwdUpdateFinalize(newPin, currentPin)
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "pwdUpdateFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun setBiokeyStart(): String {
    return try {
      requireIW().SetBiokeyStart().toString()
    } catch (e: Exception) {
      Log.e(TAG, "setBiokeyStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun setBiokeyFinalize(biokey: String, pin: String): String {
    return try {
      val result = requireIW().SetBiokeyFinalize(biokey, pin)
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "setBiokeyFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun unsetBiokeysStart(): String {
    return try {
      requireIW().UnsetBiokeysStart().toString()
    } catch (e: Exception) {
      Log.e(TAG, "unsetBiokeysStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun unsetBiokeysFinalize(pin: String): String {
    return try {
      val result = requireIW().UnsetBiokeysFinalize(pin)
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "unsetBiokeysFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun onlineOtpStart(serviceIndex: Double): String {
    return try {
      requireIW().OnlineOtpStart(serviceIndex.toInt()).toString()
    } catch (e: Exception) {
      Log.e(TAG, "onlineOtpStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun onlineOtpFinalize(serviceIndex: Double, pin: String, keyType: Double): String {
    return try {
      val result = requireIW().OnlineOtpFinalizeExt(
        serviceIndex.toInt(),
        pin,
        keyType.toLong()
      )
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "onlineOtpFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun otpAnswerOtp(): String {
    return try {
      requireIW().OtpAnswerOtp() ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "otpAnswerOtp error: ${e.message}", e)
      ""
    }
  }

  override fun otpShouldSynchronize(serviceId: Double): Boolean {
    return try {
      requireIW().OtpShouldSynchronize(serviceId.toInt()) == 1L
    } catch (e: Exception) {
      Log.e(TAG, "otpShouldSynchronize error: ${e.message}", e)
      false
    }
  }

  override fun otpModeQuery(serviceId: Double): Boolean {
    return try {
      requireIW().OtpModeQuery(serviceId.toInt()) == 1L
    } catch (e: Exception) {
      Log.e(TAG, "otpModeQuery error: ${e.message}", e)
      false
    }
  }

  override fun otpGenerate(pin: String): String {
    return try {
      val result = requireIW().OtpGenerate(pin)
      saveStorage()
      result ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "otpGenerate error: ${e.message}", e)
      ""
    }
  }

  override fun displayTime(): Double {
    return try {
      requireIW().DisplayTime().toDouble()
    } catch (e: Exception) {
      Log.e(TAG, "displayTime error: ${e.message}", e)
      0.0
    }
  }

  override fun setDeviceOS(deviceOS: String) {
    try {
      requireIW().SetDeviceOS(deviceOS)
    } catch (e: Exception) {
      Log.e(TAG, "setDeviceOS error: ${e.message}", e)
    }
  }

  override fun pushRegistrationStart(): String {
    return try {
      requireIW().PushRegistrationStart().toString()
    } catch (e: Exception) {
      Log.e(TAG, "pushRegistrationStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun pushRegistrationFinalize(pushId: String): String {
    return try {
      val result = requireIW().PushRegistrationFinalize(pushId)
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "pushRegistrationFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun checkPush(): String {
    return try {
      requireIW().CheckPush().toString()
    } catch (e: Exception) {
      Log.e(TAG, "checkPush error: ${e.message}", e)
      "-1"
    }
  }

  override fun pushAlias(): String {
    return try {
      requireIW().PushAlias() ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "pushAlias error: ${e.message}", e)
      ""
    }
  }

  override fun pushAction(): String {
    return try {
      requireIW().PushAction() ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "pushAction error: ${e.message}", e)
      ""
    }
  }

  override fun pushContext(): String {
    return try {
      requireIW().PushContext() ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "pushContext error: ${e.message}", e)
      ""
    }
  }

  override fun onlineSealStart(serviceIndex: Double): String {
    return try {
      requireIW().OnlineSealStart(serviceIndex.toInt()).toString()
    } catch (e: Exception) {
      Log.e(TAG, "onlineSealStart error: ${e.message}", e)
      "-1"
    }
  }

  override fun onlineSealFinalize(
    serviceIndex: Double,
    pin: String,
    keyType: Double,
    sealData: String
  ): String {
    return try {
      val result = requireIW().OnlineSealFinalizeExt(
        serviceIndex.toInt(),
        pin,
        keyType.toLong(),
        sealData
      )
      saveStorage()
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "onlineSealFinalize error: ${e.message}", e)
      "-1"
    }
  }

  override fun sealAnswerOtp(): String {
    return try {
      requireIW().SealAnswerOtp() ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "sealAnswerOtp error: ${e.message}", e)
      ""
    }
  }

  override fun sealShouldSynchronize(serviceId: Double): Boolean {
    return try {
      requireIW().SealShouldSynchronize(serviceId.toInt()) == 1L
    } catch (e: Exception) {
      Log.e(TAG, "sealShouldSynchronize error: ${e.message}", e)
      false
    }
  }

  override fun sealModeQuery(serviceId: Double): Boolean {
    return try {
      requireIW().SealModeQuery(serviceId.toInt()) == 1L
    } catch (e: Exception) {
      Log.e(TAG, "sealModeQuery error: ${e.message}", e)
      false
    }
  }

  override fun sealGenerate(pin: String, sealData: String): String {
    return try {
      val result = requireIW().SealGenerate(pin, sealData)
      saveStorage()
      result ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "sealGenerate error: ${e.message}", e)
      ""
    }
  }

  override fun serviceNb(): Double {
    return try {
      requireIW().ServiceNb().toDouble()
    } catch (e: Exception) {
      Log.e(TAG, "serviceNb error: ${e.message}", e)
      0.0
    }
  }

  override fun serviceName(index: Double): String {
    return try {
      requireIW().ServiceName(index.toInt()) ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "serviceName error: ${e.message}", e)
      ""
    }
  }

  override fun serviceLogo(index: Double): String {
    return try {
      requireIW().ServiceLogo(index.toInt()) ?: ""
    } catch (e: Exception) {
      Log.e(TAG, "serviceLogo error: ${e.message}", e)
      ""
    }
  }

  override fun serviceDisabled(index: Double): Boolean {
    return try {
      requireIW().ServiceDisabled(index.toInt()) == 1L
    } catch (e: Exception) {
      Log.e(TAG, "serviceDisabled error: ${e.message}", e)
      false
    }
  }

  override fun getVersionInfo(): String {
    return try {
      val result = JSONObject()
      result.put("libraryVersion", IW.VersionGet() ?: "")
      val instance = iw
      result.put("newVersionAvailable", instance?.NewVersionAvailable() ?: "")
      result.put("newVersionUrl", instance?.NewVersionURL() ?: "")
      result.put("majorVersionRequired", instance?.MajorVersionRequired() == 1L)
      result.put("shouldAskForMinorUpdate", instance?.ShouldAskForMinorUpdate() == 1L)
      result.toString()
    } catch (e: Exception) {
      Log.e(TAG, "getVersionInfo error: ${e.message}", e)
      "{}"
    }
  }

  override fun getConstants(): Map<String, Any> {
    return mapOf(
      "NAME" to NAME
    )
  }

  override fun addListener(event: String) {
    // Required for NativeEventEmitter support
  }

  override fun removeListeners(count: Double) {
    // Required for NativeEventEmitter support
  }
}
