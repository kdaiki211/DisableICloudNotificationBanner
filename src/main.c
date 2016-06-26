#define _GNU_SOURCE
#define UNICODE

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define APP_DB_BUFFER_SIZE (512 * 1024)
#define APP_DB_HKEY    (HKEY_CURRENT_USER)
#define APP_DB_SUB_KEY (TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PushNotifications\\"))
#define APP_DB_VALUE   (TEXT("AppDB"))

#define KEYWORD (TEXT("Microsoft.Explorer.Notification.{1E48D4D6-736B-3CEB-E648-CC189FC69E68}"))
#define BYTE_OFST_TO_NOTIFICATION_OPTION (522) /* offset from head of KEYWORD */

#define ICLOUD_NOTIFICATION_OFF (0x00)
#define ICLOUD_NOTIFICATION_ON  (0x20)

LONG setICloudNotificationSettingOnBuffer(BYTE* const regValue, const DWORD regSize, const BYTE val)
{
    const BYTE *const keywordPos = memmem(regValue, regSize, KEYWORD, sizeof(KEYWORD));
    BYTE* const pSw = (BYTE*)keywordPos + BYTE_OFST_TO_NOTIFICATION_OPTION;

    if ((keywordPos != NULL) && (pSw - regValue < regSize)) {
        const BYTE currentSetting = *pSw;

        /* WARNING: stop using this tool if you hit to the assert below. */
        assert((currentSetting == ICLOUD_NOTIFICATION_OFF) ||
               (currentSetting == ICLOUD_NOTIFICATION_ON));
        assert(*(pSw - 1) == 0xa4);
        assert(*(pSw - 2) == 0x0c);

        *pSw = val;

        return ERROR_SUCCESS;
    }

    return ERROR_NOT_FOUND;
}

LONG setICloudNotification(const BOOL enableNotification)
{
    const BYTE val = (enableNotification == TRUE) ? ICLOUD_NOTIFICATION_ON : ICLOUD_NOTIFICATION_OFF;
    HKEY hKey;
    
    BYTE appDbBuffer[APP_DB_BUFFER_SIZE];
    DWORD cbData = sizeof(appDbBuffer);

    LONG result;

    /* Read the registry value */
    result = RegGetValue(APP_DB_HKEY, APP_DB_SUB_KEY, APP_DB_VALUE, RRF_RT_REG_BINARY,
                         NULL, appDbBuffer, &cbData);
    if (result != ERROR_SUCCESS) {
        printf("failed to get the registry value.\n");
        goto last;
    }

    /* Modify the iCloud notification setting */
    result = setICloudNotificationSettingOnBuffer(appDbBuffer, cbData, val);
    if (result != ERROR_SUCCESS) {
        printf("keyword not found.\n");
        goto last;
    }

    /* Write the registry value */
    result = RegOpenKeyEx(APP_DB_HKEY, APP_DB_SUB_KEY, 0, KEY_SET_VALUE, &hKey);
    if (result != ERROR_SUCCESS) {
        printf("failed to open the registry key with KEY_SET_VALUE mask.\n");
        goto last;
    }

    result = RegSetValueEx(hKey, APP_DB_VALUE, 0, REG_BINARY, appDbBuffer, cbData);
    if (result != ERROR_SUCCESS) {
        printf("cannot store the registry value.\n");
        goto last;
    }

    result = RegCloseKey(hKey);
    if (result != ERROR_SUCCESS) {
        printf("cannot close the registry key.\n");
        goto last;
    }

last:
    return result;
}

int main(void)
{
    const LONG result = setICloudNotification(FALSE); /* DISABLE iCloud notification */

    if (result == ERROR_SUCCESS) {
        printf("success (restarting explorer is required)\n");
    } else {
        printf("failed\n");
        return -1;
    }

    return 0;
}

