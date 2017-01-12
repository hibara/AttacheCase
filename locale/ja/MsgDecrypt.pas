unit MsgDecrypt;

interface

resourcestring

	//���b�Z�[�W�L���v�V����
	MSG_CAPTION_ERROR                      = '�G���[';
	MSG_CAPTION_CONFIRMATION               = '�m�F';

	//�G���[���b�Z�[�W
	MSG_ERROR_FILE_OPEN                    = '��������t�@�C�����J���܂���B���̃A�v���P�[�V�������g�p���̉\��������܂��B';
	MSG_ERROR_FILE_BROKEN                  = '��������t�@�C�����J���܂���B���̈Í����t�@�C���͔j�󂳂�Ă��܂��B';
	MSG_ERROR_FILE_NOT_ATC                 = '�Í����t�@�C���ł͂���܂���B�A�^�b�V�F�P�[�X�ɂ���ĈÍ������ꂽ�t�@�C���łȂ����A'+#13+
	                                         '�t�@�C�������Ă���\��������܂��B';
	MSG_ERROR_FILE_HIGHER_VERSION          = '�f�[�^�o�[�W�������������܂��B�����ł��܂���B'+#13+
	                                         '�t�@�C���͏�ʃo�[�W�����̃A�^�b�V�F�P�[�X�ňÍ�������Ă��܂��B'+#13+
	                                         '�ŐV�ł���肵�ĕ��������݂Ă��������B';
	MSG_ERROR_FILE_LOWER_VERSION           = '�f�[�^�o�[�W�������������܂��B�����ł��܂���B'+#13+
	                                         '�t�@�C����ver.1�̃A�^�b�V�F�P�[�X�ňÍ�������Ă��܂��B'+#13+
	                                         '�Â��o�[�W�����iver.2.75�ȑO�j�������ł���肵�ĕ��������݂Ă��������B';
	MSG_ERROR_NOT_BLOWFISH_ENCRYPTION      = '�o�[�W�������������܂��B�����ł��܂���B'+#13+
	                                         '�t�@�C����ver.1�̃A�^�b�V�F�P�[�X�ňÍ�������Ă��܂���B';
	MSG_ERROR_PASSWORD_WRONG               = '�p�X���[�h���������܂��B�����ł��܂���B'+#13+
	                                         '�ꍇ�ɂ���Ă̓t�@�C�������Ă���\��������܂��B';
	MSG_ERROR_OUT_FILE_OPEN                = '�ۑ������̃t�@�C�����J���܂���B���̃A�v���P�[�V�������g�p���̉\��������܂��B';
	MSG_ERROR_OUT_FILE_WRITE               = '�ۑ������̃t�@�C���ɏ������߂܂���B�����ł��܂���B';
	MSG_ERROR_ZLIB                         = 'zlib���C�u��������G���[��Ԃ���܂����B'+#13+'�G���[�ԍ��F';
	MSG_ERROR_COMPARE_FILE                 = '�R���y�A�Ɏ��s���܂����B';
	MSG_ERROR_COMPARE_FILE_MISMATCH        = '���̃t�@�C���܂��̓t�H���_�̓��e���قȂ�悤�ł��B';
	MSG_ERROR_COMPARE_ENCRYPT_FILE         = '�Í���:';
	MSG_ERROR_COMPARE_DECRYPT_FILE         = '����  :';
	MSG_ERROR_COMPARE_NOT_READ_SOURCE      = '�R���y�A���s�����̃t�@�C����ǂݍ��ނ��Ƃ��ł��܂���B';
	MSG_ERROR_COMPARE_DATA_NOT_MATCH       = '�f�[�^�̈�v���Ȃ��t�@�C����������܂����B';
  MSG_ERROR_INVALID_FILE_PATH            = '�s���ȃt�@�C���p�X���܂܂�Ă��܂��B�����ł��܂���B';

	MSG_ERROR_DRIVE_NO_FREE_SPACE          = '���������̃h���C�u�̋󂫗e�ʂ�����܂���B'+#13+
	                                         '��������ɂ́A��%.2fMB�̋󂫗e�ʂ��K�v�ł��B���������𒆎~���܂��B';
	MSG_CONFIRM_OVER_WRITE_SAME_FILE       = '�����̃t�@�C�������ɑ��݂��Ă��܂��B�㏑�����ĕ������܂����H';
	MSG_CONFIRM_OVER_WRITE_SAME_DIR        = '�����̃t�H���_�[�����ɑ��݂��Ă��܂��B�㏑�����ĕ������܂����H';
	MSG_ERROR_NOT_OVERWRITE_MYSELF         = '�Í����t�@�C�����g�Ƀt�@�C���܂��̓t�H���_�[�𕜍����邱�Ƃ͂ł��܂���B'+#13+
	                                         '���������𒆎~���܂��B';
	MSG_CONFIRM_OPEN_DECRYPTED_FILES       = '5�ȏ�̃t�@�C���𕜍���ɊJ�����Ƃ��Ă��܂����A�����𑱂��܂����H'+#13+
	                                         '�u�������v��I������ƁA�J�����������܂��B';
	//�_�C�A���O
	DIALOG_SAVE_AS_FOLDER_CAPTION          = '�����t�H���_�[�̕ۑ�';
	DIALOG_SAVE_AS_FILE_CAPTION            = '�����t�@�C���̕ۑ�';
	DIALOG_SAVE_AS_FILTER                  = '�t�@�C���t�H���_�[|*';
	DIALOG_BUTTON_NAME_OVERWITE_YES_TO_ALL = '�S�ď㏑��(&W)'; // YesToAll
	DIALOG_BUTTON_NAME_OVERWITE_YES        = '�㏑��(&O)';     // Yes
	DIALOG_BUTTON_NAME_OVERWITE_NO         = '�ʖ��ۑ�(&A)';   // No
	DIALOG_BUTTON_NAME_OVERWITE_CANCEL     = '�L�����Z��(&C)'; // Cancel

  //���x��
	LABEL_STATUS_TITLE_DECRYPTING          = '�������Ă��܂�...';
	LABEL_STATUS_TITLE_COMPARING           = '�R���y�A���Ă��܂�...';
	LABEL_STATUS_TITLE_LISTING             = '�t�@�C�����X�g�̐���';
	LABEL_STATUS_TITLE_ERROR               = '�G���[';
	LABEL_STATUS_TITLE_USER_CANCEL         = '�L�����Z��';
	LABEL_STATUS_TITLE_COMPLETE            = '����';

	LABEL_STATUS_DETAIL_PREPARING          = '�������邽�߂̏��������Ă��܂�...';
	LABEL_STATUS_DETAIL_COMPARE_FAILED     = '�R���y�A�Ŗ����������悤�ł��B';
	LABEL_STATUS_DETAIL_FAILED             = '�����Ɏ��s���܂����B';
	LABEL_STATUS_DETAIL_STOPPED            = '���������~����܂����B';



implementation

initialization

	//Delphi�R���p�C���ɂ��œK����h��

	MSG_CAPTION_ERROR;
	MSG_CAPTION_CONFIRMATION;

	MSG_ERROR_FILE_OPEN;
	MSG_ERROR_FILE_BROKEN;
	MSG_ERROR_FILE_NOT_ATC;
	MSG_ERROR_FILE_HIGHER_VERSION;
	MSG_ERROR_FILE_LOWER_VERSION;
	MSG_ERROR_NOT_BLOWFISH_ENCRYPTION;
	MSG_ERROR_PASSWORD_WRONG;
	MSG_ERROR_OUT_FILE_OPEN;
	MSG_ERROR_OUT_FILE_WRITE;
	MSG_ERROR_ZLIB;
	MSG_ERROR_COMPARE_FILE;
	MSG_ERROR_COMPARE_FILE_MISMATCH;
	MSG_ERROR_COMPARE_ENCRYPT_FILE;
	MSG_ERROR_COMPARE_DECRYPT_FILE;
	MSG_ERROR_COMPARE_NOT_READ_SOURCE;
	MSG_ERROR_COMPARE_DATA_NOT_MATCH;


	MSG_ERROR_DRIVE_NO_FREE_SPACE;
	MSG_CONFIRM_OVER_WRITE_SAME_FILE;
	MSG_CONFIRM_OVER_WRITE_SAME_DIR;
	MSG_ERROR_NOT_OVERWRITE_MYSELF;
	MSG_CONFIRM_OPEN_DECRYPTED_FILES;
	DIALOG_SAVE_AS_FOLDER_CAPTION;
	DIALOG_SAVE_AS_FILE_CAPTION;
	DIALOG_SAVE_AS_FILTER;
	DIALOG_BUTTON_NAME_OVERWITE_YES_TO_ALL;
	DIALOG_BUTTON_NAME_OVERWITE_YES;
	DIALOG_BUTTON_NAME_OVERWITE_NO;
	DIALOG_BUTTON_NAME_OVERWITE_CANCEL;

	LABEL_STATUS_TITLE_DECRYPTING;
	LABEL_STATUS_TITLE_COMPARING;
	LABEL_STATUS_TITLE_LISTING;
	LABEL_STATUS_TITLE_ERROR;
	LABEL_STATUS_TITLE_USER_CANCEL;
	LABEL_STATUS_TITLE_COMPLETE;

	LABEL_STATUS_DETAIL_PREPARING;
	LABEL_STATUS_DETAIL_COMPARE_FAILED;
	LABEL_STATUS_DETAIL_FAILED;
	LABEL_STATUS_DETAIL_STOPPED;




end.

