import React, { useState } from "react";
import {
    Box,
    Paper,
    Stack,
    Typography,
    TextField,
    Button,
    MenuItem,
    Alert,
    Grid,
    LinearProgress,
} from "@mui/material";
import { useCurrentUser } from "../state/UserContext";
import { api } from "../api/api";

export default function ImportExportPage() {
    const { user } = useCurrentUser();
    const [file, setFile] = useState(null);
    const [format, setFormat] = useState("json");
    const [loading, setLoading] = useState(false);
    const [status, setStatus] = useState({ type: "", message: "" });

    const handleImport = async () => {
        if (!file) return setStatus({ type: "error", message: "Выберите файл для импорта" });
        try {
            setLoading(true);
            setStatus({ type: "", message: "" });

            const formData = new FormData();
            formData.append("file", file);

            const res = await api.post("/import/upload", formData, {
                headers: { "Content-Type": "multipart/form-data" },
            });

            setStatus({ type: "success", message: res.data.message });
        } catch (err) {
            console.error(err);
            setStatus({
                type: "error",
                message: err.response?.data?.message || "Ошибка при импорте",
            });
        } finally {
            setLoading(false);
        }
    };

    const handleExport = async () => {
        if (!user)
            return setStatus({ type: "error", message: "Выберите пользователя для экспорта" });

        try {
            setLoading(true);
            setStatus({ type: "", message: "" });

            const res = await api.post(
                "/export/download",
                { userId: user.id, format },
                { responseType: "blob" }
            );

            const blob = new Blob([res.data], {
                type:
                    format === "csv"
                        ? "text/csv"
                        : format === "xml"
                            ? "application/xml"
                            : "application/json",
            });

            const name = `export_${new Date().toISOString().replace(/[:.]/g, "-")}.${format}`;
            const link = document.createElement("a");
            link.href = URL.createObjectURL(blob);
            link.download = name;
            link.click();

            setStatus({ type: "success", message: `✅ Файл ${name} успешно скачан` });
        } catch (e) {
            console.error(e);
            setStatus({ type: "error", message: "Ошибка при экспорте данных" });
        } finally {
            setLoading(false);
        }
    };

    return (
        <Box sx={{ p: 4 }}>
            <Typography variant="h4" align="center" gutterBottom>
                📦 Импорт / Экспорт данных
            </Typography>

            {status.message && (
                <Alert severity={status.type} sx={{ mb: 3 }}>
                    {status.message}
                </Alert>
            )}

            {loading && <LinearProgress sx={{ mb: 2 }} />}

            <Grid container spacing={4} justifyContent="center">
                <Grid item xs={12} md={6}>
                    <Paper sx={{ p: 4, textAlign: "center" }} elevation={3}>
                        <Typography variant="h6" gutterBottom>
                            📥 Импортировать данные
                        </Typography>

                        <Typography variant="body2" sx={{ mb: 2 }}>
                            Загрузите ранее экспортированный файл (.csv, .json, .xml)
                        </Typography>

                        <Stack spacing={2} alignItems="center">
                            <input
                                type="file"
                                accept=".csv,.json,.xml"
                                onChange={(e) => setFile(e.target.files[0])}
                                style={{ marginBottom: 8 }}
                            />

                            <Button
                                variant="contained"
                                color="primary"
                                onClick={handleImport}
                                disabled={loading || !file}
                            >
                                Импортировать
                            </Button>
                        </Stack>
                    </Paper>
                </Grid>

                <Grid item xs={12} md={6}>
                    <Paper sx={{ p: 4, textAlign: "center" }} elevation={3}>
                        <Typography variant="h6" gutterBottom>
                            📤 Экспортировать данные
                        </Typography>

                        <Typography variant="body2" sx={{ mb: 2 }}>
                            Выберите формат и скачайте файл экспорта
                        </Typography>

                        <Stack spacing={2} alignItems="center">
                            <TextField
                                select
                                label="Формат экспорта"
                                value={format}
                                onChange={(e) => setFormat(e.target.value)}
                                sx={{ width: "60%" }}
                            >
                                <MenuItem value="json">JSON</MenuItem>
                                <MenuItem value="csv">CSV</MenuItem>
                                <MenuItem value="xml">XML</MenuItem>
                            </TextField>

                            <Button
                                variant="contained"
                                color="success"
                                onClick={handleExport}
                                disabled={loading}
                            >
                                Скачать экспорт
                            </Button>
                        </Stack>
                    </Paper>
                </Grid>
            </Grid>
        </Box>
    );
}